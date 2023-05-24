import { resolve } from 'path';
import { LLVM } from '@smake/llvm';

export namespace LibCoost {
  export abstract class Macro {
    static config(llvm: LLVM) {
      llvm.includedirs = [
        ...llvm.includedirs,
        resolve(__dirname, '..', 'coost', 'macro', 'include').replace(
          /\\/g,
          '/'
        ),
      ];
    }
  }
  export abstract class God {
    static config(llvm: LLVM) {
      llvm.includedirs = [
        ...llvm.includedirs,
        resolve(__dirname, '..', 'coost', 'god', 'include').replace(/\\/g, '/'),
      ];
    }
  }
  export abstract class Atomic {
    static config(llvm: LLVM) {
      llvm.includedirs = [
        ...llvm.includedirs,
        resolve(__dirname, '..', 'coost', 'atomic', 'include').replace(
          /\\/g,
          '/'
        ),
      ];
    }
  }
  export abstract class Mem {
    static config(llvm: LLVM) {
      llvm.includedirs = [
        ...llvm.includedirs,
        resolve(__dirname, '..', 'coost', 'mem', 'include').replace(/\\/g, '/'),
      ];
      llvm.files = [
        ...llvm.files,
        resolve(__dirname, '..', 'coost', 'mem', 'src', 'mem.cc').replace(
          /\\/g,
          '/'
        ),
      ];
      Macro.config(llvm);
      God.config(llvm);
      Containers.Clist.config(llvm);
      Atomic.config(llvm);
    }
  }

  export namespace Crypto {
    export abstract class MurmurHash {
      static config(llvm: LLVM) {
        llvm.includedirs = [
          ...llvm.includedirs,
          resolve(
            __dirname,
            '..',
            'coost',
            'crypto',
            'murmur_hash',
            'include'
          ).replace(/\\/g, '/'),
        ];
        llvm.files = [
          ...llvm.files,
          resolve(
            __dirname,
            '..',
            'coost',
            'crypto',
            'murmur_hash',
            'src',
            'murmur_hash.cc'
          ).replace(/\\/g, '/'),
        ];
        Macro.config(llvm);
      }
    }
  }

  export abstract class Fast {
    static config(llvm: LLVM) {
      llvm.includedirs = [
        ...llvm.includedirs,
        resolve(__dirname, '..', 'coost', 'fast', 'include').replace(
          /\\/g,
          '/'
        ),
      ];
      const filenames = ['fast.cc', 'fastring.cc'];
      llvm.files = [
        ...llvm.files,
        ...filenames.map((s) =>
          resolve(__dirname, '..', 'coost', 'fast', 'src', s).replace(
            /\\/g,
            '/'
          )
        ),
      ];
      Macro.config(llvm);
      God.config(llvm);
      Mem.config(llvm);
      Crypto.MurmurHash.config(llvm);
    }
  }

  export abstract class Time {
    static config(llvm: LLVM) {
      llvm.includedirs = [
        ...llvm.includedirs,
        resolve(__dirname, '..', 'coost', 'time', 'include').replace(
          /\\/g,
          '/'
        ),
      ];
      const filenames = ['time.cc', 'time_win.cc'];
      llvm.files = [
        ...llvm.files,
        ...filenames.map((s) =>
          resolve(__dirname, '..', 'coost', 'time', 'src', s).replace(
            /\\/g,
            '/'
          )
        ),
      ];
      Fast.config(llvm);
    }
  }

  export abstract class Rand {
    static config(llvm: LLVM) {
      llvm.includedirs = [
        ...llvm.includedirs,
        resolve(__dirname, '..', 'coost', 'rand', 'include').replace(
          /\\/g,
          '/'
        ),
      ];
      const filenames = ['rand.cc'];
      llvm.files = [
        ...llvm.files,
        ...filenames.map((s) =>
          resolve(__dirname, '..', 'coost', 'rand', 'src', s).replace(
            /\\/g,
            '/'
          )
        ),
      ];
      Fast.config(llvm);
    }
  }

  export namespace Containers {
    export abstract class Clist {
      static config(llvm: LLVM) {
        llvm.includedirs = [
          ...llvm.includedirs,
          resolve(
            __dirname,
            '..',
            'coost',
            'containers',
            'clist',
            'include'
          ).replace(/\\/g, '/'),
        ];
      }
    }
    export abstract class Array {
      static config(llvm: LLVM) {
        llvm.includedirs = [
          ...llvm.includedirs,
          resolve(
            __dirname,
            '..',
            'coost',
            'containers',
            'array',
            'include'
          ).replace(/\\/g, '/'),
        ];
        Macro.config(llvm);
        Mem.config(llvm);
      }
    }
    export abstract class Table {
      static config(llvm: LLVM) {
        llvm.includedirs = [
          ...llvm.includedirs,
          resolve(
            __dirname,
            '..',
            'coost',
            'containers',
            'table',
            'include'
          ).replace(/\\/g, '/'),
        ];
        Macro.config(llvm);
        Mem.config(llvm);
      }
    }
    export abstract class Vector {
      static config(llvm: LLVM) {
        llvm.includedirs = [
          ...llvm.includedirs,
          resolve(
            __dirname,
            '..',
            'coost',
            'containers',
            'vector',
            'include'
          ).replace(/\\/g, '/'),
        ];
        Macro.config(llvm);
        Mem.config(llvm);
      }
    }
    export abstract class List {
      static config(llvm: LLVM) {
        llvm.includedirs = [
          ...llvm.includedirs,
          resolve(
            __dirname,
            '..',
            'coost',
            'containers',
            'list',
            'include'
          ).replace(/\\/g, '/'),
        ];
        Macro.config(llvm);
        Mem.config(llvm);
      }
    }
    export abstract class Deque {
      static config(llvm: LLVM) {
        llvm.includedirs = [
          ...llvm.includedirs,
          resolve(
            __dirname,
            '..',
            'coost',
            'containers',
            'deque',
            'include'
          ).replace(/\\/g, '/'),
        ];
        Macro.config(llvm);
        Mem.config(llvm);
      }
    }
    export abstract class Compare {
      static config(llvm: LLVM) {
        llvm.includedirs = [
          ...llvm.includedirs,
          resolve(
            __dirname,
            '..',
            'coost',
            'containers',
            'compare',
            'include'
          ).replace(/\\/g, '/'),
        ];
        Macro.config(llvm);
        Mem.config(llvm);
        Crypto.MurmurHash.config(llvm);
      }
    }
    export abstract class Map {
      static config(llvm: LLVM) {
        llvm.includedirs = [
          ...llvm.includedirs,
          resolve(
            __dirname,
            '..',
            'coost',
            'containers',
            'map',
            'include'
          ).replace(/\\/g, '/'),
        ];
        Macro.config(llvm);
        Mem.config(llvm);
        Compare.config(llvm);
      }
    }
    export abstract class Set {
      static config(llvm: LLVM) {
        llvm.includedirs = [
          ...llvm.includedirs,
          resolve(
            __dirname,
            '..',
            'coost',
            'containers',
            'set',
            'include'
          ).replace(/\\/g, '/'),
        ];
        Macro.config(llvm);
        Mem.config(llvm);
        Compare.config(llvm);
      }
    }
    export abstract class UnorderedMap {
      static config(llvm: LLVM) {
        llvm.includedirs = [
          ...llvm.includedirs,
          resolve(
            __dirname,
            '..',
            'coost',
            'containers',
            'unordered_map',
            'include'
          ).replace(/\\/g, '/'),
        ];
        Macro.config(llvm);
        Mem.config(llvm);
        Compare.config(llvm);
      }
    }
    export abstract class UnorderedSet {
      static config(llvm: LLVM) {
        llvm.includedirs = [
          ...llvm.includedirs,
          resolve(
            __dirname,
            '..',
            'coost',
            'containers',
            'unordered_set',
            'include'
          ).replace(/\\/g, '/'),
        ];
        Macro.config(llvm);
        Mem.config(llvm);
        Compare.config(llvm);
      }
    }
    export abstract class LruMap {
      static config(llvm: LLVM) {
        llvm.includedirs = [
          ...llvm.includedirs,
          resolve(
            __dirname,
            '..',
            'coost',
            'containers',
            'lru_map',
            'include'
          ).replace(/\\/g, '/'),
        ];
        Macro.config(llvm);
        Mem.config(llvm);
        List.config(llvm);
        UnorderedMap.config(llvm);
      }
    }
  }

  export abstract class Error {
    static config(llvm: LLVM) {
      llvm.includedirs = [
        ...llvm.includedirs,
        resolve(__dirname, '..', 'coost', 'error', 'include').replace(
          /\\/g,
          '/'
        ),
      ];
      const filenames = ['error.cc'];
      llvm.files = [
        ...llvm.files,
        ...filenames.map((s) =>
          resolve(__dirname, '..', 'coost', 'error', 'src', s).replace(
            /\\/g,
            '/'
          )
        ),
      ];
      Macro.config(llvm);
      Fast.config(llvm);
      Containers.UnorderedMap.config(llvm);
    }
  }

  export abstract class Co {
    static config(llvm: LLVM) {
      llvm.includedirs = [
        ...llvm.includedirs,
        resolve(__dirname, '..', 'coost', 'co', 'include').replace(/\\/g, '/'),
      ];
      const filenames = [
        'co.cc',
        'io_event.cc',
        'sched.cc',
        'context/context.S',
      ];

      if (llvm.target.includes('linux')) {
        filenames.push('sock.cc');
        filenames.push('hook.cc');
        filenames.push('epoll/epoll.cc');
      } else if (llvm.target.includes('windows')) {
        filenames.push('sock_win.cc');
        filenames.push('hook_win.cc');
        filenames.push('epoll/iocp.cc');
        filenames.push('detours/detours.cpp');
        filenames.push('detours/disasm.cpp');
        filenames.push('detours/modules.cpp');
        llvm.cxflags = [
          ...llvm.cxflags,
          '-Wno-write-strings',
          '-D_WINSOCK_DEPRECATED_NO_WARNINGS',
        ];
      } else if (llvm.target.includes('darwin')) {
        filenames.push('sock.cc');
        filenames.push('hook.cc');
        filenames.push('fishhook/fishhook.c');
        filenames.push('epoll/kqueue.cc');
      }
      llvm.files = [
        ...llvm.files,
        ...filenames.map((s) =>
          resolve(__dirname, '..', 'coost', 'co', 'src', s).replace(/\\/g, '/')
        ),
      ];
      Macro.config(llvm);
      God.config(llvm);
      Mem.config(llvm);
      Fast.config(llvm);
      Rand.config(llvm);
      Time.config(llvm);
      Containers.Array.config(llvm);
      Containers.Clist.config(llvm);
      Containers.Table.config(llvm);
      Containers.Vector.config(llvm);
      Containers.Map.config(llvm);
      Containers.UnorderedMap.config(llvm);
      Containers.Set.config(llvm);
      Containers.UnorderedSet.config(llvm);
      Error.config(llvm);
    }
  }

  export abstract class Str {
    static config(llvm: LLVM) {
      llvm.includedirs = [
        ...llvm.includedirs,
        resolve(__dirname, '..', 'coost', 'str', 'include').replace(/\\/g, '/'),
      ];
      const filenames = ['str.cc'];
      llvm.files = [
        ...llvm.files,
        ...filenames.map((s) =>
          resolve(__dirname, '..', 'coost', 'str', 'src', s).replace(/\\/g, '/')
        ),
      ];
      Fast.config(llvm);
      Containers.Vector.config(llvm);
      Containers.Map.config(llvm);
      Containers.UnorderedMap.config(llvm);
      Containers.Set.config(llvm);
      Containers.UnorderedSet.config(llvm);
      Error.config(llvm);
    }
  }
}
