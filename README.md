# Orange's OS

## 实现功能

1. 多进程 Shell
    - 未知指令
    - 进程并行执行
2. 堆栈溢出
    - 跳转到恶意函数: command/attack.c
    - 跳转到堆栈执行: command/attack_stack.c
    - 缓冲区溢出: command/overflow.c
3. CRC 段完整性检测
4. Canary 检测



## 运行环境

1. 硬件: MacBook Air 2022 
2. 芯片: Apple M2 (ARM 64)
3. 系统: macOS 13.6.2
4. 交叉编译环境: aarch64 &rightarrow; x86 (i386)
5. 编译器1: x86_64-elf-gcc 13.2.0
6. 编译器2: gcc 5.4.0 (Ubuntu 22.0.4)
7. 链接器: x86_64-elf-ld 2.41



## 编译

kernel.bin 中默认附加 g1 级别的调试信息

1. mac 原生环境编译

    ~~~bash
    cd command && make install && cd .. && make image
    ~~~

2. Orbstack 编译

    - 修改 Makefile

        ~~~makefile
        CC = gcc-5
        LD = ld
        ~~~

    - 编译

        ~~~bash
        cd command && orb make install && cd .. && orb make image
        ~~~

    - 附加调试信息

        ~~~
        orb ./make
        ~~~

        执行后 kernel.debug 中为 g2 级别调试信息.



## 启动

1. 解压磁盘 (如果未解压过)

    ~~~bash
    tar -xzvf 80m.img.gz
    ~~~

2. 启动

    ~~~bash
    qemu-system-i386 -drive format=raw,file=a.img,if=floppy -drive format=raw,file=80m.img,if=ide -boot a
    ~~~



## 调试

1. 配置 VSCode

    - launch.json

        ~~~json
        {
            "version": "0.2.0",
            "configurations": [
                {
                    "name": "gdb kernel debug",
                    "type": "cppdbg",
                    "request": "launch",
                    "miDebuggerServerAddress": "127.0.0.1:1234",
                    "program": "kernel.bin",
                    "cwd": "${workspaceFolder}",
                    "environment": [],
                    "externalConsole": true,
                    "logging": {
                        "engineLogging": false
                    },
                    "stopAtConnect": true,
                    "MIMode": "gdb",
                    "miDebuggerPath": "/opt/homebrew/bin/x86_64-elf-gdb",
                },
                {
                    "name": "lldb kenel debug",
                    "type": "lldb",
                    "request": "custom",
                    "breakpointMode": "file",
                    "initCommands": [
                        "command script import ~/coding/lldb/disass.py",
                        "setting set target.x86-disassembly-flavor intel",
                    ],
                    "targetCreateCommands": [
                        "target create --arch i386 kernel.bin",
                    ],
                    "processCreateCommands": [
                        "gdb-remote localhost:1234",
                    ],
                },
            ],
        }
        ~~~

    - settings.json

        ~~~json
        {
            "lldb.displayFormat": "auto",
            "lldb.showDisassembly": "auto",
            "lldb.dereferencePointers": true,
            "lldb.consoleMode": "commands",
            "files.associations": {
                "__mutex_base": "c",
                "atomic": "c",
                "proc.h": "c",
                "locale": "c",
                "__config": "c",
                "stdio.h": "c"
            }
        }
        ~~~

    - tasks.json

        ~~~json
        {
            "version": "2.0.0",
            "tasks": [
                {
                    "label": "launch qemu",
                    "type": "shell",
                    "isBackground": true,
                    "command": "make image && echo Starting QEMU && qemu-system-i386 -drive format=raw,file=a.img,if=floppy -drive format=raw,file=80m.img,if=ide -boot a -s -S",
                    "problemMatcher": {
                        "pattern": {
                            "regexp": "^(Starting QEMU)",
                            "line": 1
                        },
                        "background": {
                            "activeOnStart": true,
                            "beginsPattern": "^(Starting QEMU)",
                            "endsPattern": "^(Starting QEMU)"
                        }
                    }
                },
                {
                    "label": "build image",
                    "type": "shell",
                    "command": "orb build image && x86_64-elf-objcopy --only-keep-debug kernel.bin kernel.debug && x86_64-elf-objcopy --strip-debug kernel.bin"
                }
            ]
        }
        ~~~

2. 启动 QEMU 等待调试器

    ~~~bash
    qemu-system-i386 -drive format=raw,file=a.img,if=floppy -drive format=raw,file=80m.img,if=ide -boot a -s -S
    ~~~

3. 启动 VSCode

    使用 lldb kenel debug 配置项启动. 若要使用 kernel.debug 中的 g2 级别调试信息, 请使用 gdb kenel debug 配置项启动, 并在调试窗口中输入如下指令加载符号文件:

    ~~~bash
    -exec add-symbol-file kernel.debug
    ~~~