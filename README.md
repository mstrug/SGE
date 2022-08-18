# SGE
Simple Game Engine for Symbian OS - native Symbian C++ framework for creating 2D and 3D games and multimedia applications. It simplifies the workflow for native Symbian application development as well as access to audio and sensors. Using Symbian 4-classes application template is not needed any more. Supports OpenGL ES and OpenVG. Includes Eclipse wizzard template plugin for easy application skeleton setup.

## Supported platforms
SGE framework works on Symbian Anna and Belle. For older Symbian versions: ^3, 5.0, 3.2, 3.1, 3.0 there are required modifications for missing platforms features (no SGE Security, Sensors, GL, etc.).

## Modules

### SGE Application 
SGE Application is the main module used to create Symbian OS SGE application. It supports simple custom-draw application and four multimedia application types: Direct Screen Access, Direct Screen Bitmap, OpenGL ES 1.1 and OpenVG. To create new application it is only required to derive from one of five base classes. There is no need for usage of Symbian Document/View classes (CAknApplication, CAknAppUi, CCoeControl, CAknDocument). SGE Application lets user to select application properties like: full screen, screen orientation support, keys blocking, red key behaviour, etc. Game dedicated classes implements standard game loop with maximum frame rate limit.

### SGE Engine 
SGE Engine module implements multi-engine game programming scheme which distinguish the menu and the game from each other in a better way than simple state variable. It gives possibility to add more virtual engines easily or use only one engine. Common objects like audio interface can be shared between engines. SGE Engine provides standard way for dispatching key/touch/system events to proper engine and provides simple layered renderer.

### SGE File 
SGE File module implements compound file using native Symbian store mechanism. It is used to pack all application files to one file maintaining folders structure. Using of compound file gives better security of the application resources and speeds up resources loading. Special tool is provided for creation of packed files on Symbian OS emulator.

### SGE String
SGE String module uses native Symbian resource mechanism for accessing multilingual strings from resource files stored on a disc drive, memory or in a SGE compound file.

### SGE Bitmap 
SGE Bitmap module provides easy way for asynchronous loading of the images and gives better support for transparent bitmaps than Symbian UI framework. Simple modifications like rotation and scale of images during loading is possible. Also transformation curve for colour-change modification is possible after file load. SGE Bitmap supports images grouping and externalization of all loaded compressed images for application start speed up. Asynchronous loading lets application UI to show progress bar which improves user experience.

### SGE Audio 
SGE Audio is the largest SGE framework module. It implements highly configurable audio interface for the games and applications. SGE Audio supports three Symbian audio interfaces which user may use as needed: simple audio playback, playback using Multimedia Framework and playback using DevSound. Using MMF or DevSound gives access to the SGE Audio track mixer. It supports unlimited tracks count, stereo or mono track mixing, volume setting for each track, events, tracks grouping (sound and music). Supported audio file types: wav, mp3. Audio mixer can be used in the main application thread or in a separate thread using Symbian IPC mechanism. 

### SGE Font 
SGE Font module encapsulates system fonts access mechanism and simplifies its usage. Various types of fonts are supported including GDR and image-based fonts.

### SGE Security 
SGE Security module implements OMA DRM 2.0 mechanism. It gives a possibility for easy verification if the application was purchased from the store or just installed from another source.

### SGE Sensors 
SGE Sensors module gives a simple way for accessing Symbian sensors: orientation, rotation, shake and accelerometer. For accelerometer data it is possible to use low pass filter for better real data projection.

### SGE GL 
SGE GL module provides object oriented encapsulation of the OpenGL ES 1.1 API. Implements texture manager for easy textures loading to the OpenGL memory including compressed textures formats: KTX and PVR and managing them during draw. It gives easy access to the Vertex arrays and Vertex buffers by simple object creation including selection of the OpenGL elements and indexing of the vertices. SGE GL implements draw to texture mechanism, texture atlases, texture based fonts, material manager, two-phase layered renderer and vertex batching mechanism for drawing optimization. 

### SGE VG 
SGE VG module provides utilities for SVG file loading and using it as fonts or images using OpenVG API.

### SGE Egl 
SGE Egl module is a submodule of the SGE Application used to create OpenGL ES and OpenVG applications. It provides selection of one of three rendering surfaces: P-buffer, Pixmap and Window.

### SGE Utils 
SGE Utils module implements simple script engine which gives ability to maintain other SGE framework modules by run-time loadable script files. It can be easily reused in any user class. SGE Utils also implements other general purpose classes like timer and active object.

## License
SGE is licensed under GPL license.

