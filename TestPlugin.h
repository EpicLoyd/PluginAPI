#pragma once
#include "game/g_public.h"

#define PLUGIN_API_VERSION 1
#define MAX_PLUGINS 3

typedef void(*xcommand_t) (void); /// xd messy, but not causing errors

typedef struct pluginExport_s {
	void*(*InitPlugin)					(int restart);
	void(*ShutDownPlugin)						(int restart);
	char *(*ClientConnect)					(int clientNum, qboolean firstTime, qboolean isBot);
	int(*ClientBegin)						(int clientNum, qboolean allowTeamReset);
	qboolean(*ClientUserinfoChanged)			(int clientNum);
	void(*ClientDisconnect)					(int clientNum);
	int(*ClientCommand)					(int clientNum);
	void(*ClientThink)						(int clientNum, usercmd_t *ucmd);
	void(*RunFrame)							(int levelTime);
	qboolean(*ConsoleCommand)					(void);
	int(*Call)                              (int cmd, int numargs, va_list *args);
} pluginExport_t;

typedef struct plugininfo_s{
	char *name;
	char *author;
	char *version;
	qboolean *usegamelibrary;
	pluginExport_t *export;
} plugininfo_t;

typedef struct plugin_s {
	char		name[MAX_QPATH];
	char        author;
	char        version;
	void		*dllHandle;
	int         id;
	// fill the import/export tables
	void *		(*GetPluginAPI)(int apiVersion, ...);
	qboolean usegamelibrary;
	qboolean paused;
} plugin_t;

typedef struct pluginImport_s {
	// misc
	void(*Print)								(const char *msg, ...);
	void(*Error)								(int level, const char *error, ...);
	int(*Milliseconds)							(void);
	void(*PrecisionTimerStart)					(void **timer);
	int(*PrecisionTimerEnd)					(void *timer);
	void(*SV_RegisterSharedMemory)				(char *memory);
	int(*RealTime)								(qtime_t *qtime);
	void(*TrueMalloc)							(void **ptr, int size);
	void(*TrueFree)								(void **ptr);
	void(*SnapVector)							(float *v);

	// cvar
	void(*Cvar_Register)						(vmCvar_t *vmCvar, const char *varName, const char *defaultValue, uint32_t flags);
	void(*Cvar_Set)								(const char *var_name, const char *value, uint32_t flags);
	void(*Cvar_Update)							(vmCvar_t *vmCvar);
	int(*Cvar_VariableIntegerValue)			(const char *var_name);
	void(*Cvar_VariableStringBuffer)			(const char *var_name, char *buffer, int bufsize);

	// cmd
	int(*Argc)									(void);
	void(*Argv)									(int n, char *buffer, int bufferLength);

	// filesystem
	void(*FS_Close)								(fileHandle_t f);
	int(*FS_GetFileList)						(const char *path, const char *extension, char *listbuf, int bufsize);
	int(*FS_Open)								(const char *qpath, fileHandle_t *f, fsMode_t mode);
	int(*FS_Read)								(void *buffer, int len, fileHandle_t f);
	int(*FS_Write)								(const void *buffer, int len, fileHandle_t f);

	// server
	void(*AdjustAreaPortalState)				(sharedEntity_t *ent, qboolean open);
	qboolean(*AreasConnected)						(int area1, int area2);
	int(*DebugPolygonCreate)					(int color, int numPoints, vec3_t *points);
	void(*DebugPolygonDelete)					(int id);
	void(*DropClient)							(int clientNum, const char *reason);
	int(*EntitiesInBox)						(const vec3_t mins, const vec3_t maxs, int *list, int maxcount);
	qboolean(*EntityContact)						(const vec3_t mins, const vec3_t maxs, const sharedEntity_t *ent, int capsule);
	void(*GetConfigstring)						(int num, char *buffer, int bufferSize);
	qboolean(*GetEntityToken)						(char *buffer, int bufferSize);
	void(*GetServerinfo)						(char *buffer, int bufferSize);
	void(*GetUsercmd)							(int clientNum, usercmd_t *cmd);
	void(*GetUserinfo)							(int num, char *buffer, int bufferSize);
	qboolean(*InPVS)								(const vec3_t p1, const vec3_t p2);
	qboolean(*InPVSIgnorePortals)					(const vec3_t p1, const vec3_t p2);
	void(*LinkEntity)							(sharedEntity_t *ent);
	void(*LocateGameData)						(sharedEntity_t *gEnts, int numGEntities, int sizeofGEntity_t, playerState_t *clients, int sizeofGClient);
	int(*PointContents)						(const vec3_t point, int passEntityNum);
	void(*SendConsoleCommand)					(int exec_when, const char *text);
	void(*SendServerCommand)					(int clientNum, const char *text);
	void(*SetBrushModel)						(sharedEntity_t *ent, const char *name);
	void(*SetConfigstring)						(int num, const char *string);
	void(*SetServerCull)						(float cullDistance);
	void(*SetUserinfo)							(int num, const char *buffer);
	void(*SiegePersSet)							(siegePers_t *pers);
	void(*SiegePersGet)							(siegePers_t *pers);
	void(*Trace)								(trace_t *results, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, int passEntityNum, int contentmask, int capsule, int traceFlags, int useLod);
	void(*UnlinkEntity)							(sharedEntity_t *ent);
	qhandle_t(*R_RegisterSkin)						(const char *name);

	void(*G2API_ListModelBones)					(void *ghlInfo, int frame);
	void(*G2API_ListModelSurfaces)				(void *ghlInfo);
	qboolean(*G2API_HaveWeGhoul2Models)				(void *ghoul2);
	void(*G2API_SetGhoul2ModelIndexes)			(void *ghoul2, qhandle_t *modelList, qhandle_t *skinList);
	qboolean(*G2API_GetBoltMatrix)					(void *ghoul2, const int modelIndex, const int boltIndex, mdxaBone_t *matrix, const vec3_t angles, const vec3_t position, const int frameNum, qhandle_t *modelList, vec3_t scale);
	qboolean(*G2API_GetBoltMatrix_NoReconstruct)	(void *ghoul2, const int modelIndex, const int boltIndex, mdxaBone_t *matrix, const vec3_t angles, const vec3_t position, const int frameNum, qhandle_t *modelList, vec3_t scale);
	qboolean(*G2API_GetBoltMatrix_NoRecNoRot)		(void *ghoul2, const int modelIndex, const int boltIndex, mdxaBone_t *matrix, const vec3_t angles, const vec3_t position, const int frameNum, qhandle_t *modelList, vec3_t scale);
	int(*G2API_InitGhoul2Model)				(void **ghoul2Ptr, const char *fileName, int modelIndex, qhandle_t customSkin, qhandle_t customShader, int modelFlags, int lodBias);
	qboolean(*G2API_SetSkin)						(void *ghoul2, int modelIndex, qhandle_t customSkin, qhandle_t renderSkin);
	int(*G2API_Ghoul2Size)						(void *ghlInfo);
	int(*G2API_AddBolt)						(void *ghoul2, int modelIndex, const char *boneName);
	void(*G2API_SetBoltInfo)					(void *ghoul2, int modelIndex, int boltInfo);
	qboolean(*G2API_SetBoneAngles)					(void *ghoul2, int modelIndex, const char *boneName, const vec3_t angles, const int flags, const int up, const int right, const int forward, qhandle_t *modelList, int blendTime, int currentTime);
	qboolean(*G2API_SetBoneAnim)					(void *ghoul2, const int modelIndex, const char *boneName, const int startFrame, const int endFrame, const int flags, const float animSpeed, const int currentTime, const float setFrame, const int blendTime);
	qboolean(*G2API_GetBoneAnim)					(void *ghoul2, const char *boneName, const int currentTime, float *currentFrame, int *startFrame, int *endFrame, int *flags, float *animSpeed, int *modelList, const int modelIndex);
	void(*G2API_GetGLAName)						(void *ghoul2, int modelIndex, char *fillBuf);
	int(*G2API_CopyGhoul2Instance)				(void *g2From, void *g2To, int modelIndex);
	void(*G2API_CopySpecificGhoul2Model)		(void *g2From, int modelFrom, void *g2To, int modelTo);
	void(*G2API_DuplicateGhoul2Instance)		(void *g2From, void **g2To);
	qboolean(*G2API_HasGhoul2ModelOnIndex)			(void *ghlInfo, int modelIndex);
	qboolean(*G2API_RemoveGhoul2Model)				(void *ghlInfo, int modelIndex);
	qboolean(*G2API_RemoveGhoul2Models)				(void *ghlInfo);
	void(*G2API_CleanGhoul2Models)				(void **ghoul2Ptr);
	void(*G2API_CollisionDetect)				(CollisionRecord_t *collRecMap, void *ghoul2, const vec3_t angles, const vec3_t position, int frameNumber, int entNum, vec3_t rayStart, vec3_t rayEnd, vec3_t scale, int traceFlags, int useLod, float fRadius);
	void(*G2API_CollisionDetectCache)			(CollisionRecord_t *collRecMap, void* ghoul2, const vec3_t angles, const vec3_t position, int frameNumber, int entNum, vec3_t rayStart, vec3_t rayEnd, vec3_t scale, int traceFlags, int useLod, float fRadius);
	qboolean(*G2API_SetRootSurface)					(void *ghoul2, const int modelIndex, const char *surfaceName);
	qboolean(*G2API_SetSurfaceOnOff)				(void *ghoul2, const char *surfaceName, const int flags);
	qboolean(*G2API_SetNewOrigin)					(void *ghoul2, const int boltIndex);
	qboolean(*G2API_DoesBoneExist)					(void *ghoul2, int modelIndex, const char *boneName);
	int(*G2API_GetSurfaceRenderStatus)			(void *ghoul2, const int modelIndex, const char *surfaceName);
	void(*G2API_AbsurdSmoothing)				(void *ghoul2, qboolean status);
	void(*G2API_SetRagDoll)						(void *ghoul2, sharedRagDollParams_t *params);
	void(*G2API_AnimateG2Models)				(void *ghoul2, int time, sharedRagDollUpdateParams_t *params);
	qboolean(*G2API_RagPCJConstraint)				(void *ghoul2, const char *boneName, vec3_t min, vec3_t max);
	qboolean(*G2API_RagPCJGradientSpeed)			(void *ghoul2, const char *boneName, const float speed);
	qboolean(*G2API_RagEffectorGoal)				(void *ghoul2, const char *boneName, vec3_t pos);
	qboolean(*G2API_GetRagBonePos)					(void *ghoul2, const char *boneName, vec3_t pos, vec3_t entAngles, vec3_t entPos, vec3_t entScale);
	qboolean(*G2API_RagEffectorKick)				(void *ghoul2, const char *boneName, vec3_t velocity);
	qboolean(*G2API_RagForceSolve)					(void *ghoul2, qboolean force);
	qboolean(*G2API_SetBoneIKState)					(void *ghoul2, int time, const char *boneName, int ikState, sharedSetBoneIKStateParams_t *params);
	qboolean(*G2API_IKMove)							(void *ghoul2, int time, sharedIKMoveParams_t *params);
	qboolean(*G2API_RemoveBone)						(void *ghoul2, const char *boneName, int modelIndex);
	void(*G2API_AttachInstanceToEntNum)			(void *ghoul2, int entityNum, qboolean server);
	void(*G2API_ClearAttachedInstance)			(int entityNum);
	void(*G2API_CleanEntAttachments)			(void);
	qboolean(*G2API_OverrideServer)					(void *serverInstance);
	void(*G2API_GetSurfaceName)					(void *ghoul2, int surfNumber, int modelIndex, char *fillBuf);
	void(*Call)                              (char *pluginName, int cmd, int numargs, ...);
} pluginImport_t;


enum ReturnStatus{
	PLUGIN_NULL = 0,
	PLUGIN_COUNTINUE,
	PLUGIN_STOP,
};


Q_EXPORT plugininfo_t* QDECL GetPluginAPI(int apiVersion, pluginImport_t *import);