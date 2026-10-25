# NiceDevin AudioLocal

本地离线音频转换器：转换全程在本机完成，不上传任何文件。

FFmpeg 驱动、**两段式转换**：先把文件转写到临时区，点「导出」才真正落到你选择的目标文件夹。

## 平台版本

| 目录 | 平台 | 技术栈 | 状态 |
|------|------|--------|------|
| `windows_qt/` | Windows | Qt 6（C++ / Qt Widgets） | ✅ 可用 |
| `android_kotlin/` | Android | Kotlin / Jetpack Compose | ✅ 可用 |

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

## android_kotlin 构建

- 环境：Android Studio 2026.2.1+（自带 JBR JDK 25）、Android SDK（compileSdk/targetSdk 37，minSdk 24）
- 技术栈：AGP 9.4.0（内置 Kotlin 2.4.21）+ Jetpack Compose（Material3 1.4.0）
- 依赖：转换内核用 Maven Central 维护分支 `io.github.maitrungduc1410:ffmpeg-kit-audio:7.1.5`（在线拉取，无需本地 ffmpeg）
- 构建（工程自带 Gradle wrapper 9.6.0）：

  ```
  gradlew.bat :app:assembleDebug
  ```

  产物：`android_kotlin/app/build/outputs/apk/debug/app-debug.apk`（约 50 MB）

  release（正式签名版；签名凭据存于本机 `keystore.properties`，属私密信息，未入库）：

  ```
  gradlew.bat :app:assembleRelease
  ```

  产物：`android_kotlin/app/build/outputs/apk/release/app-release.apk`

- **Android 原生库修复说明**：`ffmpeg-kit-audio:7.1.5` 这个 fork 的 `libffmpegkit.so` 缺失对 `libc++_shared.so` 的 DT_NEEDED 依赖，在 arm64 / Android 14+ 真机上加载 `libavcodec.so` 时报 `cannot locate symbol __gxx_personality_v0`，导致转换点击失败。修复：用 LIEF 给 `libffmpegkit.so`（arm64）与 `libffmpegkit_armv7a_neon.so`（armeabi-v7a）补上该依赖，放入 `android_kotlin/app/src/main/jniLibs/<abi>/`，并在 `app/build.gradle.kts` 开启 `useLegacyPackaging` 且用 `pickFirsts` 让本地补丁库优先。补丁脚本见 `.tools_ffmpeg/patch_ffmpegkit_cpp_deps.py`（已 gitignore）。

- 启动：用 Android Studio 打开 `android_kotlin/` 目录，等待首次 Gradle 同步完成，连接真机或模拟器后运行 `app` 配置；也可以直接把上面的 debug APK 装到手机。
- 移动端差异（与网页/桌面版一致，排除手机/平板不支持的内容）：拖拽改为点击调用系统文件选择器；「下载」改为调用系统保存对话框（SAF，无需存储权限）。界面等同网页版——单页滚动、暖米纸色 + 琥珀强调、中英文切换。

## 许可证

[MIT](LICENSE) © 2026 NiceDevin
