#pragma once

#include "XorStr.h"

#pragma once

#define M_PI 3.1415926535897931

//not using this anymore to get players
#define EFT_OBJECTMANAGER 0x151A218

//#define RUST_GAMEASSEMBLY 0x23FBF30 
//#define GAMEASSEMBLY_TEMP1 0xB8
//#define TEMP1_ENTITYREALM 0x0

#define OBJECTMANAGER_TAGGEDLISTFIRST 0x8
#define OBJECTMANAGER_TAGGEDLIST 0x0
#define OBJECTMANAGER_ACTIVELIST 0x18
#define OBJECTMANAGER_ACTIVELISTLAST 0x10

#define TAGGEDLIST_NEXTTAGGEDENT 0x8
#define BASEENT_GAMEOBJECT 0x10

#define GAMEOBJECT_OBJECTCLASS 0x30
#define GAMEOBJECT_OBJECTNAMEPTR 0x60 // PlayerSuperior(Clone)
#define GAMEOBJECT_TAG 0x54

//READ PLAYER LIST HERE
#define GAMEOBJECT_V1 0x30
#define V1_V2 0x18
#define V2_MLOCALGAMEWORLDPOINT 0x28
#define MLOCALGAMEWORLDPOINT_MREGISTERPLAYERPOINT 0x78
#define MREGFISTERPLAYERPOINT_MPLAYERSCOUNTPOINT 0x18 // read this as int and check player number
#define MREGISTERPLAYERPOINT_V1 0x10
#define MPLAYERSCOUNTPOINT_OFFSET 0x20 // each is 0x8 apart

#define UNKNOWNENTITY_FIRST 0x10
#define FIRST_GAMEOBJ 0x30 // regular game obj.

//0x7ffdd3930000
#define OBJECTCLASS_TRANSFORM 0x8
#define OBJECTCLASS_ENTITYPTR 0x18
#define ENTITYPTR_BASEENT 0x28
#define TRANSFORM_VISUALSTATE 0x38
#define VISUALSTATE_POSITION 0xb0

#define ENTITYPTR_MATRIX 0xD8

#define UNKNOWNENTITY_PLAYERPROFILE 0x3D8
#define UNKNOWNENTITY_LOCALPLAYERCHECK 0x18
#define UNKNOWNENTITY_PROCEDURALWEAPONANIMATION 0x168

#define PROCEDUALWEAPONANIMATION_SHOTEFFECTOR 0x48
#define SHOTEFFECTOR_INTENSITY 0x68

#define PLAYERPROFILE_PLAYERINFO 0x28
#define PLAYERINFO_PLAYERNAME 0x10
#define PLAYERNAME_WCHAR 0x14

#define PLAYERPROFILE_HEALTH 0x40
#define HEALTH_HEALTHINFO 0x18
#define HEALTHINFO_CURRENT 0x10 // float
#define HEALTHINFO_MAX 0x14

#define UNKNOWNENTITY_HEALTHCONTROLLER 0x3D0
#define HEALTHCONTROLLER_HEALTHBODY 0x18
#define HEALTHBODY_BODYCONTROLLER 0x28

//FIX BASEENT OFFSET
//#define BASEENT_NAME 0x4E0
//#define BASEENT_MODEL 0xA8
//#define BASEENT_FLAG 0x478

#define UNKNOWNENTITY_MODEL 0xA0
#define MODEL_MODEL1 0x28
#define MODEL1_MODEL2 0x28
#define MODEL2_MODEL3 0x10
#define MODEL3_OFFSET 0x20
#define BONE_WORLD 0x10  //and then do the transform thing and you are done
//
//#define MODEL_TRANSFORM 0x40
//
//#define TRANSFORM_BONE 0x20


//#define BASENETWORKABLE_CLIENTENTITIES 0x10
//#define CLIENTENTITIES_ENTITY_LIST 0x28
//#define ENTITY_LIST_SIZE 0x18
//#define ENTITY_LIST_OBJECTS_LIST 0x10
//#define OBJECTS_LIST_OFFSET 0x20 // first is localplayer
//#define UNKNOWNENTITY_FIRST 0x10
//#define FIRST_GAMEOBJ 0x30 // regular game obj.

//& sets state, | 

enum PlayerFlags
{
	Unused1 = 1,
	Unused2 = 2,
	IsAdmin = 4,
	ReceivingSnapshot = 8,
	Sleeping = 16,
	Spectating = 32,
	Wounded = 64,
	IsDeveloper = 128,
	Connected = 256,
	VoiceMuted = 512,
	ThirdPersonViewmode = 1024,
	EyesViewmode = 2048,
	ChatMute = 4096,
	NoSprint = 8192,
	Aiming = 16384,
	DisplaySash = 32768,
	Relaxed = 65536,
	SafeZone = 131072,
	Workbench1 = 1048576,
	Workbench2 = 2097152,
	Workbench3 = 4194304
};