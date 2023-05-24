const { LLVM } = require('@smake/llvm');
const { vscode } = require('@smake/llvm-vscode');
const { LibCoost } = require('./lib');

const targets = [
  'aarch64-apple-darwin',
  // 'aarch64-linux-gnu',
  // 'x86_64-pc-windows-msvc',
];

const builds = targets.map(target => {
  const test = new LLVM('coost-containers-test', target);
  test.files = ['test/containers.cxx'];
  LibCoost.Containers.Array.config(test);
  LibCoost.Containers.Clist.config(test);
  LibCoost.Containers.Table.config(test);
  LibCoost.Containers.Vector.config(test);
  LibCoost.Containers.List.config(test);
  LibCoost.Containers.Deque.config(test);
  LibCoost.Containers.Map.config(test);
  LibCoost.Containers.LruMap.config(test);
  vscode(test);

  return test;
})
module.exports = builds;