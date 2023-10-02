# Minirenderer

**Minirenderer** is a tiny, simple CPU rasterization rendingring program.

It is suitable for those who attempts to learn the basis of 3D-rendering and computer graphics.

Currently it is only tested on windows-VisualStudio 2022/2019. It supports .png files as input/output textures and .fbx files as input 3D model.

![](blob:file:///849bc782-a0b3-4c74-8715-6bab3776d281) ![](README_md_files/f2385190-614c-11ee-9ed6-8f5a811404c5.jpeg?v=1&type=image)

### **How to build:**

Because fbx is not an open-source file format, you need to download the fbxsdk at <https://www.autodesk.com/developer-network/platform-technologies/fbx-sdk-2020-0> and configure the project. FBX SDK 2020.0.1 VS2017 is recommended.

First, download and install the sdk as instructions.

![](README_md_files/563737b0-613e-11ee-a1c5-2153c1a8046e.jpeg?v=1&type=image)

Then, Configure the environment as instructions below.(Release Mode). If you prefer other configuration, please get reference at <https://help.autodesk.com/view/FBX/2020/ENU/?guid=FBX_Developer_Help_getting_started_installing_and_configuring_configuring_the_fbx_sdk_for_wind_html>

![](README_md_files/1e5b5500-613f-11ee-a1c5-2153c1a8046e.jpeg?v=1&type=image)![](README_md_files/6d932da0-613f-11ee-a1c5-2153c1a8046e.jpeg?v=1&type=image)![](README_md_files/8b905530-613f-11ee-a1c5-2153c1a8046e.jpeg?v=1&type=image)![](README_md_files/a9e47660-613f-11ee-a1c5-2153c1a8046e.jpeg?v=1&type=image)

### **Things to notice:**

1.  Please pay attention that your fbx files are in correct coordinate. A good fbx comparing to a bad fbx is shown below. Here's the principle: Keep your model not rotated and scaled in -Z forward, Y up, right coordinate system. The Minirenderer shares the same convention with openGL. And if you are using blender, below is the export settings your can refer to. Do not forget to apply all rotations and scales when exporting. Other 3D softwares are similar. And don't forget to unwrap model's UV\~

![](README_md_files/1ad25bb0-6147-11ee-a1c5-2153c1a8046e.jpeg?v=1&type=image)

1.  Use 2^n \* 2^n (e.g 512 pixel \* 512 pixel) squared png files as input/output texture.

### Pros and Cons:

Pros:

1.  Every basic render pipelines and shaders are listed and modularized. You can read every details by your self and modify them easily.

2.  CPU-based. You don't have to get trapped in the complicated GPU Api like OpenGL.

Cons:

1.  Now it only support .png files and .fbx files.

2.  While it is CPU-based, the render efficiency will be lower than GPU.

### Contact the author:

If you need any help or want to communicate with me, leave a message here or contact me by the ways below:

google mail: <mustard.green.1112@gmail.com>

bilibili account: 红芥 66

Instagram: mustard_1112

You are always welcome!&#x20;

I love computer graphics!
