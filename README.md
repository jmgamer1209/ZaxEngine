# ZaxEngine

该项目的目标，从渲染出发，逐步构建出完整的游戏引擎。

引擎名取自FF7中的角色扎克斯，象征着荣誉与梦想。

关于每个版本的变化情况，查看 Versions 目录，为了方便后续的查看，[Versions目录](./Versions)会包含所有版本信息。

由于这不是个扫盲文档，所以每个版本信息，基本上是一个知识点的梳理，深入的内容，需要查阅其他相关资料。<br>
如果后期发现前面版本的文档有错误，只会在最新的提交上去修改版本内容。

此文档只记录核心内容。

引擎引入的第三方插件和库：
* imgui v1.90，配置过程参考示例中的 example_glfw_opengl3，glfw 是一个多平台窗口管理器，同时因为项目初始使用 OpenGL API，所以此界面插件也采用此图形库。
* glad2 v2.0.4，用于运行时查找 opengl 函数。github 需要选择 glad2 分支。库要在网站生成，选项选择 gl 3.3，勾选 loader。注意是 glad2 而不是旧版的 glad
* stb_image，图片加载库
* glm，数学库
* assimp v5.3.1, 资源模型导入库，比如 obj，fbx

引擎编译工具：VS2022，这里没有选择CMake，目前主要开发还是在PC平台，到后期如果有跨平台需要再进行切换。

C++版本：C++14

初始版本使用 OpenGL 图形库,版本为 3.3（此版本可以包含大部分内容了，如果后面功能不足，会进行版本升级），后续随着扩展，会逐步扩充 DirectX11，DirectX12，Vulkan，当然，这是当前版本稳定后的事情了。

坐标系当前采用右手坐标系，屏幕为xy方向，垂直面向你的方向为正z方向。z向与Unity坐标相反。有些文章说 OpenGL 是右手坐标系，这是不对的，只是旧的固定管线为右手，而现代管线是自己来定的。这里采用右手坐标系，因为glm库的默认以右手坐标系来生成矩阵的。

当前版本的画面：

![](./Versions/Assets/v0.9_result.png)