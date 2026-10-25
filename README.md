# NiceDevin AudioLocal

本地离线音频转换器：转换全程在本机完成，不上传任何文件。

FFmpeg 驱动、**两段式转换**：先把文件转写到临时区，点「导出」才真正落到你选择的目标文件夹。

## 平台版本

| 目录 | 平台 | 技术栈 | 状态 |
|------|------|--------|------|
| `windows_qt/` | Windows | Qt 6（C++ / Qt Widgets） | ✅ 可用 |
| `android_kotlin/` | Android | Kotlin | 🚧 规划中 |

## 功能

- 拖拽或浏览添加音频文件（可批量、自动过滤非音频、去重）
- 格式互转：可**输出** mp3 / flac / wav / m4a / ogg / opus；可**输入** mp3 / flac / wav / m4a / aac / ogg / opus / amr / wma / ape / wv
- 两段式转换：先到临时区，点「导出」才落到目标目录；未导出会被清掉
- 批量转换、实时进度、单个失败自动续跑
- 安装包中英文界面

## windows_qt 构建

- 环境：VS2022 + Qt 6.7.3（msvc2019_64）+ CMake + Ninja
- 依赖：`windows_qt/bin/ffmpeg.exe`（体积大，未入库，需自行放入）
- 构建：

  ```
  build.cmd
  ```

  产物：`windows_qt/build/AudioLocalQt.exe`

- 安装包（Inno Setup）脚本：`windows_qt/installer/audiolocal.iss`

## 许可证

[MIT](LICENSE) © 2026 NiceDevin
