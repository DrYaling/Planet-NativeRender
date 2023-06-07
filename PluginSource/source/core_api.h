#pragma once
/*
auto generated c header, do not modify this
*/
#include <stdint.h>
#include "type_wrapper.h"
#include "PlatformBase.h"
typedef int32_t* I32Ptr;
typedef uint32_t* U32Ptr;
typedef const int32_t* CI32Ptr;
typedef const uint32_t* CU32Ptr;
typedef char* CharPtr;
typedef unsigned char* UCharPtr;
typedef const char* CCharPtr;
typedef unsigned const char* UCCharPtr;
extern "C"{
    int32_t FtCreateBattle(unsigned const char*/*wraper of *const u8*/ buffer, uint32_t len);
    bool FtDestroyBattle();
    int32_t FtLoadConfig(int32_t config_type, unsigned const char*/*wraper of *const u8*/ content, uint32_t len);
    void FtUnloadAllConfigs();
    bool FtPointFree(uint32_t map, int32_t x, int32_t y);
    int32_t FtEnterBattle(uint32_t team);
    int32_t FtStartBattle(uint32_t team, CVoidPtr/*wraper of *mut *const u8*/ out_buffer, int32_t*/*wraper of *mut i32*/ out_len, uint32_t*/*wraper of *mut u32*/ out_cap);
    int32_t FtPrepareBattle(uint32_t team, unsigned const char*/*wraper of *const u8*/ buffer, uint32_t len, CVoidPtr/*wraper of *mut *const u8*/ out_buffer, int32_t*/*wraper of *mut i32*/ out_len, uint32_t*/*wraper of *mut u32*/ out_cap);
    int32_t FtMoveTo(unsigned const char*/*wraper of *const u8*/ buffer, uint32_t len, CVoidPtr/*wraper of *mut *const u8*/ out_buffer, int32_t*/*wraper of *mut i32*/ out_len, uint32_t*/*wraper of *mut u32*/ out_cap);
    int32_t FtGetPathToPoint(uint64_t uuid, int32_t target_x, int32_t target_y, CVoidPtr/*wraper of *mut *const Position*/ out_buffer, int32_t*/*wraper of *mut i32*/ out_len, uint32_t*/*wraper of *mut u32*/ out_cap);
    int32_t FtGetPathToEnemy(uint64_t uuid, uint64_t target_id, uint32_t skill_id, int32_t v_x, int32_t v_y, CVoidPtr/*wraper of *mut *const Position*/ out_buffer, int32_t*/*wraper of *mut i32*/ out_len, uint32_t*/*wraper of *mut u32*/ out_cap);
    int32_t FtGetPathToSkillTarget(uint64_t uuid, uint32_t skill_id, int32_t target_x, int32_t target_y, int32_t v_x, int32_t v_y, CVoidPtr/*wraper of *mut *const Position*/ out_buffer, int32_t*/*wraper of *mut i32*/ out_len, uint32_t*/*wraper of *mut u32*/ out_cap);
    int32_t FtGetTargetDistance(uint64_t uuid, uint64_t target_id);
    int32_t FtGetDistanceTo(uint64_t uuid, int32_t target_x, int32_t target_y, bool path_distance);
#if UNITY_WIN || UNITY_OSX
    bool FtSetBattleAiHook(StateHookCallback/*wraper of ai::behavior::StateHookCallback*/ func);
#endif
    bool FtBattleAiUpdate();
    int32_t FtCastSkill(unsigned const char*/*wraper of *const u8*/ buffer, uint32_t len, CVoidPtr/*wraper of *mut *const u8*/ out_buffer, int32_t*/*wraper of *mut i32*/ out_len, uint32_t*/*wraper of *mut u32*/ out_cap);
#if UNITY_WIN || UNITY_OSX
    void FtPrintPoint(int32_t x, int32_t y);
#endif
    void FtBattleUpdateTurn();
    int32_t FtTakeRecords(CVoidPtr/*wraper of *mut *const u8*/ out_buffer, int32_t*/*wraper of *mut i32*/ out_len, uint32_t*/*wraper of *mut u32*/ out_cap);
#if UNITY_WIN || UNITY_OSX
    bool FtReloadAi(uint64_t uuid);
#endif
    int32_t FtGetRoleAttribute(uint64_t uuid, int32_t attr);
    int32_t FtSkillPredict(unsigned const char*/*wraper of *const u8*/ buffer, uint32_t len, CVoidPtr/*wraper of *mut *const u8*/ out_buffer, int32_t*/*wraper of *mut i32*/ out_len, uint32_t*/*wraper of *mut u32*/ out_cap);
    int32_t FtReqEvent(unsigned const char*/*wraper of *const u8*/ buffer, uint32_t len, CVoidPtr/*wraper of *mut *const u8*/ out_buffer, int32_t*/*wraper of *mut i32*/ out_len, uint32_t*/*wraper of *mut u32*/ out_cap);
    int32_t FtCheckSkillValid(uint64_t uuid, uint32_t skill_id, uint16_t cast_pos, uint64_t target_id, bool point_target);
    int32_t FtStatusSynchronize(unsigned const char*/*wraper of *const u8*/ buffer, uint32_t len);
    int32_t FtGetValidTargets(uint64_t uuid, uint32_t skill_id, CVoidPtr/*wraper of *mut *const u64*/ targets, int32_t*/*wraper of *mut i32*/ out_len, uint32_t*/*wraper of *mut u32*/ out_cap);
    int32_t FtGetAttackers(uint64_t uuid, uint32_t point_id, CVoidPtr/*wraper of *mut *const u64*/ targets, int32_t*/*wraper of *mut i32*/ out_len, uint32_t*/*wraper of *mut u32*/ out_cap);
    bool SetRustLogLevel(int32_t log_level);
    int32_t Encrypt(unsigned char*/*wraper of *mut u8*/ data, uint32_t len, int32_t input, CVoidPtr/*wraper of *mut *mut u8*/ out_buffer, int32_t*/*wraper of *mut i32*/ out_len, uint32_t*/*wraper of *mut u32*/ out_cap);
    int32_t Decrypt(unsigned char*/*wraper of *mut u8*/ data, uint32_t len, int32_t input, CVoidPtr/*wraper of *mut *mut u8*/ out_buffer, int32_t*/*wraper of *mut i32*/ out_len, uint32_t*/*wraper of *mut u32*/ out_cap);
    void MainUpdate();
    void RsOnApplicationQuit();
    int64_t TimestampMills();
    int32_t Compress(unsigned char*/*wraper of *mut u8*/ data, uint32_t len, uint32_t format, uint32_t level, CVoidPtr/*wraper of *mut *mut u8*/ out_buffer, int32_t*/*wraper of *mut i32*/ out_len, uint32_t*/*wraper of *mut u32*/ out_cap);
    int32_t Decompress(unsigned char*/*wraper of *mut u8*/ data, uint32_t len, CVoidPtr/*wraper of *mut *mut u8*/ out_buffer, int32_t*/*wraper of *mut i32*/ out_len, uint32_t*/*wraper of *mut u32*/ out_cap);
    void ReleaseVector(unsigned char*/*wraper of *mut u8*/ ptr, uint32_t size, uint32_t type_size, uint32_t cap);
    void BindDebugLogger(UnityLogCallback/*wraper of UnityLogCallback*/ func);
#if UNITY_WIN || UNITY_OSX
    bool PackLua(char*/*wraper of *mut c_char*/ lua_path, char*/*wraper of *mut c_char*/ target_path, bool compress, bool encrypt);
#endif
    int32_t LoadLua(const char*/*wraper of *const c_char*/ lua_path);
    void UnloadLua();
    int32_t LoadFile(const char*/*wraper of *const c_char*/ filename, CVoidPtr/*wraper of *mut *mut u8*/ out_buffer, int32_t*/*wraper of *mut i32*/ out_len, uint32_t*/*wraper of *mut u32*/ out_cap);
    void ExpInverseDirection(int32_t dx, int32_t dy, int32_t*/*wraper of *mut i32*/ x, int32_t*/*wraper of *mut i32*/ y);
    bool IsBaseDir(int32_t x, int32_t y);
    int32_t GetHexId(int32_t x, int32_t y, uint32_t map);
    void GetHexPosition(uint32_t id, uint32_t map, int32_t*/*wraper of *mut i32*/ x, int32_t*/*wraper of *mut i32*/ y);
    uint16_t GetMapSize(uint32_t map);
    void ExpDestroyMap();
    bool ExpCreateMap(uint32_t width, uint32_t height, uint32_t*/*wraper of *mut u32*/ barrier_ptr, uint32_t barrier_count);
    int32_t ExpFindPath(int32_t start_x, int32_t start_y, int32_t target_x, int32_t target_y, uint32_t distance, CVoidPtr/*wraper of *mut *mut Position*/ out_buffer, int32_t*/*wraper of *mut i32*/ out_len, uint32_t*/*wraper of *mut u32*/ out_cap);
    void ExpUpdateGridState(uint32_t grid, bool block, bool explore_map);
    int32_t ExpGetDistance(int32_t x, int32_t y, int32_t target_x, int32_t target_y, bool path_distance);
    void ExpRemoveFogs(uint32_t*/*wraper of *mut u32*/ list, uint32_t list_count);
    void ExpAddFogs(uint32_t*/*wraper of *mut u32*/ list, uint32_t list_count);
    void ExpAddAllFogsExceptDistance(int32_t x, int32_t y, uint32_t distance);
    void ExpClearAllFogs();
    void NetCloseSession(uint32_t session);
    void NetCloseNetwork();
    bool NetCreateSession(const char*/*wraper of *const i8*/ host, uint16_t port, int32_t session_type, NetworkEventCallback/*wraper of NetworkEventCallback*/ event_handler);
    bool NetSendMsg(uint32_t session_id, unsigned const char*/*wraper of *const u8*/ msg, uint32_t len, uint16_t opcode, uint16_t sub_code, uint32_t rpc);
    int32_t GetSessionState(uint32_t session_id);
    bool NetInitWorker(uint32_t worker);
#if UNITY_WIN || UNITY_OSX
    bool EncryptAssetBundle(char*/*wraper of *mut c_char*/ ab_path, char*/*wraper of *mut c_char*/ target_path, char*/*wraper of *mut c_char*/ ext);
#endif
    bool SetAbRoot(char*/*wraper of *mut c_char*/ ab_path);
    CVoidPtr/*wraper of *mut AssetBundleStream*/ LoadAb(char*/*wraper of *mut c_char*/ ab_path);
    uint32_t AbReadBytes(CVoidPtr/*wraper of *mut AssetBundleStream*/ stream, unsigned char*/*wraper of *mut u8*/ buffer, uint32_t size, uint32_t offset);
    uint32_t AbStreamLen(CVoidPtr/*wraper of *mut AssetBundleStream*/ stream);
    void ReleaseRefPtr(void*/*wraper of *mut c_void*/ ptr);
}