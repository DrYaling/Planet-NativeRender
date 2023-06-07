/*
auto generated c header, do not modify this
*/
#include "core_api.h"
#include "Unity/IUnityInterface.h"
extern "C"{
    int32_t UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CaFtLoadConfig(int32_t config_type, unsigned const char*/*wraper of *const u8*/ content, uint32_t len){
        return FtLoadConfig(config_type, content, len);
    }
#if UNITY_WIN || UNITY_OSX
    bool UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CaFtSetBattleAiHook(StateHookCallback/*wraper of ai::behavior::StateHookCallback*/ func){
        return FtSetBattleAiHook(func);
    }
#endif
    bool UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CaFtBattleAiUpdate(){
        return FtBattleAiUpdate();
    }
#if UNITY_WIN || UNITY_OSX
    void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CaFtPrintPoint(int32_t x, int32_t y){
        FtPrintPoint(x, y);
    }
#endif
#if UNITY_WIN || UNITY_OSX
    bool UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CaFtReloadAi(uint64_t uuid){
        return FtReloadAi(uuid);
    }
#endif
    int32_t UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CaEncrypt(unsigned char*/*wraper of *mut u8*/ data, uint32_t len, int32_t input, CVoidPtr/*wraper of *mut *mut u8*/ out_buffer, int32_t*/*wraper of *mut i32*/ out_len, uint32_t*/*wraper of *mut u32*/ out_cap){
        return Encrypt(data, len, input, out_buffer, out_len, out_cap);
    }
    int32_t UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CaDecrypt(unsigned char*/*wraper of *mut u8*/ data, uint32_t len, int32_t input, CVoidPtr/*wraper of *mut *mut u8*/ out_buffer, int32_t*/*wraper of *mut i32*/ out_len, uint32_t*/*wraper of *mut u32*/ out_cap){
        return Decrypt(data, len, input, out_buffer, out_len, out_cap);
    }
    void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CaMainUpdate(){
        MainUpdate();
    }
    void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CaRsOnApplicationQuit(){
        RsOnApplicationQuit();
    }
    int32_t UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CaCompress(unsigned char*/*wraper of *mut u8*/ data, uint32_t len, uint32_t format, uint32_t level, CVoidPtr/*wraper of *mut *mut u8*/ out_buffer, int32_t*/*wraper of *mut i32*/ out_len, uint32_t*/*wraper of *mut u32*/ out_cap){
        return Compress(data, len, format, level, out_buffer, out_len, out_cap);
    }
    int32_t UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CaDecompress(unsigned char*/*wraper of *mut u8*/ data, uint32_t len, CVoidPtr/*wraper of *mut *mut u8*/ out_buffer, int32_t*/*wraper of *mut i32*/ out_len, uint32_t*/*wraper of *mut u32*/ out_cap){
        return Decompress(data, len, out_buffer, out_len, out_cap);
    }
    void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CaReleaseVector(unsigned char*/*wraper of *mut u8*/ ptr, uint32_t size, uint32_t type_size, uint32_t cap){
        ReleaseVector(ptr, size, type_size, cap);
    }
#if UNITY_WIN || UNITY_OSX
    bool UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CaPackLua(char*/*wraper of *mut c_char*/ lua_path, char*/*wraper of *mut c_char*/ target_path, bool compress, bool encrypt){
        return PackLua(lua_path, target_path, compress, encrypt);
    }
#endif
    int32_t UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CaLoadLua(const char*/*wraper of *const c_char*/ lua_path){
        return LoadLua(lua_path);
    }
    void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CaUnloadLua(){
        UnloadLua();
    }
    int32_t UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CaLoadFile(const char*/*wraper of *const c_char*/ filename, CVoidPtr/*wraper of *mut *mut u8*/ out_buffer, int32_t*/*wraper of *mut i32*/ out_len, uint32_t*/*wraper of *mut u32*/ out_cap){
        return LoadFile(filename, out_buffer, out_len, out_cap);
    }
#if UNITY_WIN || UNITY_OSX
    bool UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CaEncryptAssetBundle(char*/*wraper of *mut c_char*/ ab_path, char*/*wraper of *mut c_char*/ target_path, char*/*wraper of *mut c_char*/ ext){
        return EncryptAssetBundle(ab_path, target_path, ext);
    }
#endif
    bool UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CaSetAbRoot(char*/*wraper of *mut c_char*/ ab_path){
        return SetAbRoot(ab_path);
    }
    CVoidPtr UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CaLoadAb(char*/*wraper of *mut c_char*/ ab_path){
        return LoadAb(ab_path);
    }
    uint32_t UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CaAbReadBytes(CVoidPtr/*wraper of *mut AssetBundleStream*/ stream, unsigned char*/*wraper of *mut u8*/ buffer, uint32_t size, uint32_t offset){
        return AbReadBytes(stream, buffer, size, offset);
    }
    uint32_t UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CaAbStreamLen(CVoidPtr/*wraper of *mut AssetBundleStream*/ stream){
        return AbStreamLen(stream);
    }
    void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CaReleaseRefPtr(void*/*wraper of *mut c_void*/ ptr){
        ReleaseRefPtr(ptr);
    }
}