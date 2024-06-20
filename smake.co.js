const { LLVM } = require('@smake/llvm');
const { vscode } = require('@smake/llvm-vscode');
const { LibCoost } = require('./lib');

const targets = [
  'aarch64-apple-darwin',
  // 'aarch64-linux-gnu',
  'x86_64-pc-windows-msvc',
];

const builds = targets.map(target => {
  const test = new LLVM('coost-co-test', target);
  test.files = ['test/co.cxx'];
  LibCoost.Co.config(test);
  if (test.target.includes('linux')) {
    test.libs = [
      ...test.libs,
      'pthread', 'dl',
    ]
  }
  test.cxflags = [
    ...test.cxflags,
    // '-D_COOST_CO_DISABLE_HOOK',
    // '-D_COOST_CO_ENABLE_SCHEDLOG',
    '-D_COOST_CO_ENABLE_HOOKLOG',
  ];
  vscode(test);

  return test;
})
module.exports = builds;