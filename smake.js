const { LLVM } = require('@smake/llvm');
const { vscode } = require('@smake/llvm-vscode');
const { globSync } = require('glob');

const target = 'aarch64-apple-darwin';
// const target = 'aarch64-linux-gnu';
// const target = 'x86_64-pc-windows-msvc';
const test = new LLVM('coost-core', target);
if (target.includes('darwin')) {
  test.files = [
    ...globSync('./src/**/*.cc'),
    ...globSync('./src/**/*.c'),
    'src/co/context/context.S',
  ];
} else if (target.includes('windows')) {
  test.files = [
    ...globSync('./src/**/*.cc'),
    'src/co/detours/detours.cpp',
    'src/co/detours/disasm.cpp',
    'src/co/detours/modules.cpp',
    'src/co/context/context.S',
  ];
} else {
  test.files = [
    ...globSync('./src/**/*.cc'),
    'src/co/context/context.S',
  ];
}
test.includedirs = [
  ...test.includedirs,
  'include',
];
test.cxflags = [
  ...test.cxflags,
  '-D_CO_DISABLE_HOOK',
];

if (target.includes('linux')) {
  test.ldflags = [
    ...test.ldflags,
    '-pthread',
    '-ldl',
  ];
}

vscode(test);

module.exports = [test];