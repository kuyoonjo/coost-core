const { LLVM } = require('@smake/llvm');
const { vscode } = require('@smake/llvm-vscode');
const { LibCoost } = require('./lib');

const targets = [
  // 'aarch64-apple-darwin',
  // 'aarch64-linux-gnu',
  'x86_64-pc-windows-msvc',
];

const builds = targets.map(target => {
  const test = new LLVM('coost-mem-test', target);
  test.files = ['test/mem.cxx'];
  LibCoost.Mem.config(test);
  vscode(test);

  return test;
})
module.exports = builds;