# ZaxEngine

这是一款用于学习用途的渲染引擎

引擎名取自FF7中的角色扎克斯，象征着荣誉与梦想。

关于每个版本的变化情况，查看 Versions 目录，为了方便后续的查看，[Versions目录](./Versions)会包含所有版本信息。

此文档只记录核心内容。

引擎使用的插件：
* imgui v1.90，配置过程参考示例中的 example_glfw_opengl3，glfw 是一个多平台窗口管理器，同时因为项目初始使用 OpenGL API，所以此界面插件也采用此图形库。

引擎编译工具：VS2022

C++版本：C++14

初始版本使用 OpenGL 图形库，后续随着扩展，会逐步扩充 DirectX11，DirectX12，Vulkan，当然，这是当前版本稳定后的事情了。
