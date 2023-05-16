const { LLVM } = require('@smake/llvm');
const { vscode } = require('@smake/llvm-vscode');
const { globSync } = require('glob');

const target = 'aarch64-apple-darwin';
const test = new LLVM('coost-core', target);
test.files = [
  ...globSync('./src/**/*.cc'),
  ...globSync('./src/**/*.c'),
  'src/co/context/context.S',
];
test.includedirs = [
  ...test.includedirs,
  'include',
];
vscode(test);

module.exports = [test];