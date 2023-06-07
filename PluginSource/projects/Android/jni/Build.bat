echo "copy android lib" 
echo off
call ndk-build -s
copy ..\libs\arm64-v8a\libRenderingPlugin.so E:\WorkSpace\project\Assets\Plugins\Android\libs\arm64-v8a
copy ..\libs\armeabi-v7a\libRenderingPlugin.so E:\WorkSpace\project\Assets\Plugins\Android\libs\armeabi-v7a
echo "build complete"