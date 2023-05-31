const { LLVM } = require('@smake/llvm');
const { vscode } = require('@smake/llvm-vscode');
const { LibCoost } = require('./lib');
const { resolve } = require('path');

const targets = [
  'aarch64-apple-darwin',
  // 'aarch64-linux-gnu',
  // 'x86_64-pc-windows-msvc',
];

const builds = targets.map(target => {
  const test = new LLVM('coost-log-test', target);
  test.files = ['test/log.cxx'];
  LibCoost.Log.config(test);
  if (test.target.includes('linux')) {
    test.libs = [
      ...test.libs,
      'pthread', 'dl',
    ]
  }
  test.cxflags = [
    ...test.cxflags,
    `-D_COOST_LOG_HEADER=${resolve(__dirname)}/test/log.h`,
  ];
  vscode(test);

  return test;
})
module.exports = builds;