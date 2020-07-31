#include "Game-Data.h"
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

std::vector<Player> Game_Data::_Player_Vector = std::vector<Player>();
std::vector<Player> Game_Data::_Obj_Vector = std::vector<Player>();
std::vector<Player> Game_Data::_Animal_Vector = std::vector<Player>();
std::vector<Player> Game_Data::_Turret_Vector = std::vector<Player>();
std::vector<Player> Game_Data::_Vehicle_Vector = std::vector<Player>();
ULONG64 Game_Data::_Camera_Addr = 0;
Player Game_Data::localplayer = Player();

ULONG64 _Test_ULONG64 = 0;
INT16 _Test_INT16 = 0;

ULONG64 mono_field_static_get_value(ULONG64 klass, ULONG offset)
{
	ULONG64 vTable = Memory::_This->Read_Memory<ULONG64>(klass + 0xD0);
	if (vTable != NULL)
	{
		vTable = Memory::_This->Read_Memory<ULONG64>(vTable + 0x8);
		if (vTable != NULL)
		{
			ULONG vTableSize = 0x40 + Memory::_This->Read_Memory<ULONG>(klass + 0x5C) * 0x8;
			return Memory::_This->Read_Memory<ULONG64>(vTable + vTableSize + offset);
		}
	}
	return 0;
}

int size__ = 0;
ULONG64 gameObj = 0;

bool getSize()
{
	if (gameObj == 0)
	{
		size__ = 0;
		return false;
	}
	ULONG64 _V1 = Memory::_This->Read_Memory(gameObj + GAMEOBJECT_V1, _Test_ULONG64);
	ULONG64 _V2 = Memory::_This->Read_Memory(_V1 + V1_V2, _Test_ULONG64);
	ULONG64 mLocalGameWorldPoint = Memory::_This->Read_Memory(_V2 + V2_MLOCALGAMEWORLDPOINT, _Test_ULONG64);
	ULONG64 mRegisterPlayerPoint = Memory::_This->Read_Memory(mLocalGameWorldPoint + MLOCALGAMEWORLDPOINT_MREGISTERPLAYERPOINT, _Test_ULONG64);

	//player count
	ULONG64 mPlayersCountPoint = Memory::_This->Read_Memory(mRegisterPlayerPoint + MREGFISTERPLAYERPOINT_MPLAYERSCOUNTPOINT, _Test_ULONG64);
	ULONG _Test_ULONG = 0;
	size__ = Memory::_This->Read_Memory(mPlayersCountPoint, _Test_ULONG);
	return true;
}

ULONG64 getBaseNetworked()
{
	ULONG64 ObjectManager = Memory::_This->Read_Memory(Memory::_This->Base + EFT_OBJECTMANAGER, _Test_ULONG64);
	ULONG64 taggedObjectsPtr = Memory::_This->Read_Memory(ObjectManager + OBJECTMANAGER_ACTIVELIST, _Test_ULONG64);
	ULONG64 firstObject = Memory::_This->Read_Memory(ObjectManager + OBJECTMANAGER_ACTIVELISTLAST, _Test_ULONG64);

	ULONG64 BaseNetworkable;

	while (firstObject != taggedObjectsPtr)
	{
		Player currentPlayer = Player();
		currentPlayer.BaseObject = taggedObjectsPtr;
		currentPlayer.GameObject = Memory::_This->Read_Memory(taggedObjectsPtr + BASEENT_GAMEOBJECT, _Test_ULONG64);

		ULONG64 objectNamePor = Memory::_This->Read_Memory(currentPlayer.GameObject + GAMEOBJECT_OBJECTNAMEPTR, _Test_ULONG64);
		char world_name_char[100] = { 0 };
		Memory::_This->Read_Memory(objectNamePor, world_name_char, 50);
		//printf("The name is: %s at mem pos %llu\n", world_name_char, objectNamePor);

		if (strstr(world_name_char, "GameWorld")) // this is the path to succ
		{
			//ULONG64 objClass = Memory::_This->Read_Memory(currentPlayer.GameObject + GAMEOBJECT_OBJECTCLASS, _Test_ULONG64);
			//ULONG64 entity = Memory::_This->Read_Memory(objClass + OBJECTCLASS_ENTITYPTR, _Test_ULONG64);
			//ULONG64 base = Memory::_This->Read_Memory(entity + ENTITYPTR_BASEENT, _Test_ULONG64);

			////need the first
			//ULONG64 temp1 = Memory::_This->Read_Memory(base, _Test_ULONG64);
			//temp1 = Memory::_This->Read_Memory(temp1, _Test_ULONG64);
			//ULONG64 temp2 = Memory::_This->Read_Memory(temp1 + 0x30, _Test_ULONG64);
			//ULONG64 temp3 = Memory::_This->Read_Memory(temp2 + 0x30, _Test_ULONG64);
			//ULONG64 temp4 = Memory::_This->Read_Memory(temp3 + 0x30, _Test_ULONG64);

			//BaseNetworkable = mono_field_static_get_value(temp4, 0);

			////again, need the first
			//BaseNetworkable = Memory::_This->Read_Memory(BaseNetworkable, _Test_ULONG64);
			gameObj = currentPlayer.GameObject;
			ULONG64 _V1 = Memory::_This->Read_Memory(currentPlayer.GameObject + GAMEOBJECT_V1, _Test_ULONG64);
			ULONG64 _V2 = Memory::_This->Read_Memory(_V1 + V1_V2, _Test_ULONG64);
			ULONG64 mLocalGameWorldPoint = Memory::_This->Read_Memory(_V2 + V2_MLOCALGAMEWORLDPOINT, _Test_ULONG64);
			ULONG64 mRegisterPlayerPoint = Memory::_This->Read_Memory(mLocalGameWorldPoint + MLOCALGAMEWORLDPOINT_MREGISTERPLAYERPOINT, _Test_ULONG64);

			//player count
			ULONG64 mPlayersCountPoint = Memory::_This->Read_Memory(mRegisterPlayerPoint + MREGFISTERPLAYERPOINT_MPLAYERSCOUNTPOINT, _Test_ULONG64);
			ULONG _Test_ULONG = 0;
			size__ = Memory::_This->Read_Memory(mPlayersCountPoint, _Test_ULONG);

			ULONG64 __V1 = Memory::_This->Read_Memory(mRegisterPlayerPoint + MREGISTERPLAYERPOINT_V1, _Test_ULONG64);

			BaseNetworkable = __V1;


			return BaseNetworkable;
			//ULONG64 t1 = Memory::_This->Read_Memory(BaseNetworkable + 0x10, _Test_ULONG64);
			//ULONG64 t2 = Memory::_This->Read_Memory(t1 + 0x28, _Test_ULONG64);
			//networked_list = Memory::_This->Read_Memory(t2 + 0x10, _Test_ULONG64);
		}
		taggedObjectsPtr = Memory::_This->Read_Memory(taggedObjectsPtr + TAGGEDLIST_NEXTTAGGEDENT, _Test_ULONG64);
	}


	taggedObjectsPtr = Memory::_This->Read_Memory(ObjectManager + OBJECTMANAGER_ACTIVELISTLAST, _Test_ULONG64);
	firstObject = Memory::_This->Read_Memory(ObjectManager + OBJECTMANAGER_ACTIVELIST, _Test_ULONG64);
	//must have fucked up somewhere
	while (firstObject != taggedObjectsPtr)
	{
		Player currentPlayer = Player();
		currentPlayer.BaseObject = taggedObjectsPtr;
		currentPlayer.GameObject = Memory::_This->Read_Memory(taggedObjectsPtr + BASEENT_GAMEOBJECT, _Test_ULONG64);

		ULONG64 objectNamePor = Memory::_This->Read_Memory(currentPlayer.GameObject + GAMEOBJECT_OBJECTNAMEPTR, _Test_ULONG64);
		char world_name_char[100] = { 0 };
		Memory::_This->Read_Memory(objectNamePor, world_name_char, 50);
		//printf("The name is: %s at mem pos %llu\n", world_name_char, objectNamePor);

		if (strstr(world_name_char, "GameWorld")) // this is the path to succ
		{
			//ULONG64 objClass = Memory::_This->Read_Memory(currentPlayer.GameObject + GAMEOBJECT_OBJECTCLASS, _Test_ULONG64);
			//ULONG64 entity = Memory::_This->Read_Memory(objClass + OBJECTCLASS_ENTITYPTR, _Test_ULONG64);
			//ULONG64 base = Memory::_This->Read_Memory(entity + ENTITYPTR_BASEENT, _Test_ULONG64);

			////need the first
			//ULONG64 temp1 = Memory::_This->Read_Memory(base, _Test_ULONG64);
			//temp1 = Memory::_This->Read_Memory(temp1, _Test_ULONG64);
			//ULONG64 temp2 = Memory::_This->Read_Memory(temp1 + 0x30, _Test_ULONG64);
			//ULONG64 temp3 = Memory::_This->Read_Memory(temp2 + 0x30, _Test_ULONG64);
			//ULONG64 temp4 = Memory::_This->Read_Memory(temp3 + 0x30, _Test_ULONG64);

			//BaseNetworkable = mono_field_static_get_value(temp4, 0);

			////again, need the first
			//BaseNetworkable = Memory::_This->Read_Memory(BaseNetworkable, _Test_ULONG64);
			gameObj = currentPlayer.GameObject;
			ULONG64 _V1 = Memory::_This->Read_Memory(currentPlayer.GameObject + GAMEOBJECT_V1, _Test_ULONG64);
			ULONG64 _V2 = Memory::_This->Read_Memory(_V1 + V1_V2, _Test_ULONG64);
			ULONG64 mLocalGameWorldPoint = Memory::_This->Read_Memory(_V2 + V2_MLOCALGAMEWORLDPOINT, _Test_ULONG64);
			ULONG64 mRegisterPlayerPoint = Memory::_This->Read_Memory(mLocalGameWorldPoint + MLOCALGAMEWORLDPOINT_MREGISTERPLAYERPOINT, _Test_ULONG64);

			//player count
			ULONG64 mPlayersCountPoint = Memory::_This->Read_Memory(mRegisterPlayerPoint + MREGFISTERPLAYERPOINT_MPLAYERSCOUNTPOINT, _Test_ULONG64);
			ULONG _Test_ULONG = 0;
			size__ = Memory::_This->Read_Memory(mPlayersCountPoint, _Test_ULONG);

			ULONG64 __V1 = Memory::_This->Read_Memory(mRegisterPlayerPoint + MREGISTERPLAYERPOINT_V1, _Test_ULONG64);

			BaseNetworkable = __V1;


			return BaseNetworkable;
			//ULONG64 t1 = Memory::_This->Read_Memory(BaseNetworkable + 0x10, _Test_ULONG64);
			//ULONG64 t2 = Memory::_This->Read_Memory(t1 + 0x28, _Test_ULONG64);
			//networked_list = Memory::_This->Read_Memory(t2 + 0x10, _Test_ULONG64);
		}
		taggedObjectsPtr = Memory::_This->Read_Memory(taggedObjectsPtr + TAGGEDLIST_NEXTTAGGEDENT, _Test_ULONG64);
	}
	return 0;
}

ULONG64 getCam()
{
	std::vector<Player> _Player_Vector_Temp;

	ULONG64 ObjectManager = Memory::_This->Read_Memory(Memory::_This->Base + EFT_OBJECTMANAGER, _Test_ULONG64);
	ULONG64 taggedObjectsPtr = Memory::_This->Read_Memory(ObjectManager + OBJECTMANAGER_TAGGEDLIST, _Test_ULONG64);
	ULONG64 firstObject = Memory::_This->Read_Memory(ObjectManager + OBJECTMANAGER_TAGGEDLISTFIRST, _Test_ULONG64);

	while (firstObject != taggedObjectsPtr)
	{
		Player currentPlayer = Player();
		currentPlayer.BaseObject = taggedObjectsPtr;
		currentPlayer.GameObject = Memory::_This->Read_Memory(taggedObjectsPtr + BASEENT_GAMEOBJECT, _Test_ULONG64);
		currentPlayer.tag = Memory::_This->Read_Memory(currentPlayer.GameObject + GAMEOBJECT_TAG, _Test_INT16);

		if (currentPlayer.tag == 5)
		{
			//mainCamera
			ULONG64 objClass = Memory::_This->Read_Memory(currentPlayer.GameObject + GAMEOBJECT_OBJECTCLASS, _Test_ULONG64);
			ULONG64 entity = Memory::_This->Read_Memory(objClass + OBJECTCLASS_ENTITYPTR, _Test_ULONG64);
			ULONG64 transform = Memory::_This->Read_Memory(objClass + OBJECTCLASS_TRANSFORM, _Test_ULONG64);

			return entity;
		}
		taggedObjectsPtr = Memory::_This->Read_Memory(taggedObjectsPtr + TAGGEDLIST_NEXTTAGGEDENT, _Test_ULONG64);
	}

	//we must have not found it
	taggedObjectsPtr = Memory::_This->Read_Memory(ObjectManager + OBJECTMANAGER_TAGGEDLISTFIRST, _Test_ULONG64);
	firstObject = Memory::_This->Read_Memory(ObjectManager + OBJECTMANAGER_TAGGEDLIST, _Test_ULONG64);

	while (firstObject != taggedObjectsPtr)
	{
		Player currentPlayer = Player();
		currentPlayer.BaseObject = taggedObjectsPtr;
		currentPlayer.GameObject = Memory::_This->Read_Memory(taggedObjectsPtr + BASEENT_GAMEOBJECT, _Test_ULONG64);
		currentPlayer.tag = Memory::_This->Read_Memory(currentPlayer.GameObject + GAMEOBJECT_TAG, _Test_INT16);

		if (currentPlayer.tag == 5)
		{
			//mainCamera
			ULONG64 objClass = Memory::_This->Read_Memory(currentPlayer.GameObject + GAMEOBJECT_OBJECTCLASS, _Test_ULONG64);
			ULONG64 entity = Memory::_This->Read_Memory(objClass + OBJECTCLASS_ENTITYPTR, _Test_ULONG64);
			ULONG64 transform = Memory::_This->Read_Memory(objClass + OBJECTCLASS_TRANSFORM, _Test_ULONG64);

			return entity;
		}
		taggedObjectsPtr = Memory::_This->Read_Memory(taggedObjectsPtr + TAGGEDLIST_NEXTTAGGEDENT, _Test_ULONG64);
	}

	return 0;
}

Player getLocal(ULONG64 *List, ULONG64 length)
{
	//#define UNKNOWNENTITY_PROCEDURALWEAPONANIMATION 0x70
//#define PROCEDUALWEAPONANIMATION_SHOTEFFECTOR 0x48
//#define SHOTEFFECTOR_INTENSITY 0x68

	for (int i = 0; i < length; i++)
	{
		Player currentPlayer = Player();
		ULONG64 UnknownEnt = List[i];
		ULONG64 temp1 = Memory::_This->Read_Memory(UnknownEnt + UNKNOWNENTITY_FIRST, _Test_ULONG64);
		ULONG64 obj = Memory::_This->Read_Memory(temp1 + FIRST_GAMEOBJ, _Test_ULONG64);

		ULONG64 localplayerCheck = Memory::_This->Read_Memory(UnknownEnt + UNKNOWNENTITY_LOCALPLAYERCHECK, _Test_ULONG64);

		if (localplayerCheck > 0)
		{

			currentPlayer.Unknown = UnknownEnt;
			currentPlayer.GameObject = obj;
			currentPlayer.tag = Memory::_This->Read_Memory(currentPlayer.GameObject + GAMEOBJECT_TAG, _Test_INT16);
			ULONG64 objectNamePor = Memory::_This->Read_Memory(currentPlayer.GameObject + GAMEOBJECT_OBJECTNAMEPTR, _Test_ULONG64);
			char world_name_char[100] = { 0 };
			Memory::_This->Read_Memory(objectNamePor, world_name_char, 50);
			//other stuff
			//printf("\nlocal local at position %llu", obj);
			Game_Data::localplayer = currentPlayer;
			currentPlayer.name = world_name_char;
			currentPlayer.objName = "Superior";
			ULONG64 objClass = Memory::_This->Read_Memory(currentPlayer.GameObject + GAMEOBJECT_OBJECTCLASS, _Test_ULONG64);
			ULONG64 entity = Memory::_This->Read_Memory(objClass + OBJECTCLASS_ENTITYPTR, _Test_ULONG64);
			ULONG64 base = Memory::_This->Read_Memory(entity + ENTITYPTR_BASEENT, _Test_ULONG64);
			INT _Test_INT = 0;
			ULONG64 transform = Memory::_This->Read_Memory(objClass + OBJECTCLASS_TRANSFORM, _Test_ULONG64);
			currentPlayer.VisualState = Memory::_This->Read_Memory(transform + TRANSFORM_VISUALSTATE, _Test_ULONG64);
			return currentPlayer;
		}
	}
	//printf("You fucked up!");
}

bool Game_Data::Hook_Start()
{
	_Memory->Init();
	//_Memory->Dump_Names();
	//exit(0);
	_Rusty_Addr = Memory::_This->Get_Model_Addr();

	BaseNetworkable = getBaseNetworked();

	_Camera_Addr = getCam();

	return true;
}

void Game_Data::Get_Data(bool box, bool animal, bool turret, bool vehicle)
{
	if (BaseNetworkable == 0 || size__ <= 1) // this shit is constant
	{
		//printf("%llu %i", BaseNetworkable, size__);
		BaseNetworkable = getBaseNetworked();
		_Player_Vector.clear();
		_Obj_Vector.clear();
		_Animal_Vector.clear();
		_Turret_Vector.clear();
		_Vehicle_Vector.clear();
		return;
	}

	//BaseNetworkable = getBaseNetworked();
	//printf("%llu", BaseNetworkable);

	//size__ = Memory::_This->Read_Memory(mPlayersCountPoint, _Test_ULONG);
	if (!getSize()) return; // must have  fucked up

	_Camera_Addr = getCam(); 
	//printf("%llu", _Camera_Addr);

	ULONG64 list = BaseNetworkable;
	list += 0x20; // skip some junk

	//printf("before copying\n");
	int size = size__;
	_Networked_List = (ULONG64*)malloc(size * sizeof(ULONG64));

	//printf("Nice, you didn't fuck up\n");

	Memory::_This->Read_Memory(list, _Networked_List, size * sizeof(ULONG64));
	_Networked_size = size;

	printf("copying successful, size: %llu _Networked_List first: %llu\n", size, list);


	localplayer = getLocal(_Networked_List, _Networked_size);

	//walk the list

	//first is localplayer
	_Player_Vector.clear();
	_Obj_Vector.clear();
	_Animal_Vector.clear();
	_Turret_Vector.clear();
	_Vehicle_Vector.clear();
	for (ULONG64 i = 1; i < size; i++)
	{
		//if (_Networked_List[i] == 0) continue;
		Player currentPlayer = Player();

		currentPlayer.Unknown = _Networked_List[i];
		ULONG64 temp1 = Memory::_This->Read_Memory(_Networked_List[i] + UNKNOWNENTITY_FIRST, _Test_ULONG64);
		ULONG64 obj = Memory::_This->Read_Memory(temp1 + FIRST_GAMEOBJ, _Test_ULONG64);

		currentPlayer.GameObject = obj;
		currentPlayer.tag = Memory::_This->Read_Memory(currentPlayer.GameObject + GAMEOBJECT_TAG, _Test_INT16);

		//Player
		ULONG64 objClass = Memory::_This->Read_Memory(currentPlayer.GameObject + GAMEOBJECT_OBJECTCLASS, _Test_ULONG64);
		ULONG64 entity = Memory::_This->Read_Memory(objClass + OBJECTCLASS_ENTITYPTR, _Test_ULONG64);
		ULONG64 transform = Memory::_This->Read_Memory(objClass + OBJECTCLASS_TRANSFORM, _Test_ULONG64);

		//printf("\n before local model \n");

		//ULONG64 model =Memory::_This->Read_Memory(entity + ENTITYPTR_MODEL, _Test_ULONG64);
		//ULONG64 modelTransform =Memory::_This->Read_Memory(model + MODEL_TRANSFORM, _Test_ULONG64);
		//ULONG64 head =Memory::_This->Read_Memory(modelTransform + TRANSFORM_BONE + 47 * 8, _Test_ULONG64);
		//ULONG64 headWorld =Memory::_This->Read_Memory(head + BONE_WORLD, _Test_ULONG64);

		//printf("\n before bone func \n");
		//currentPlayer.head = get_bone_pos(headWorld);
		//printf("\n after bone func \n");
		currentPlayer.VisualState = Memory::_This->Read_Memory(transform + TRANSFORM_VISUALSTATE, _Test_ULONG64);

		ULONG64 objectNamePor = Memory::_This->Read_Memory(currentPlayer.GameObject + GAMEOBJECT_OBJECTNAMEPTR, _Test_ULONG64);
		char world_name_char[100] = { 0 };
		Memory::_This->Read_Memory(objectNamePor, world_name_char, 50);
		//printf("The name is: %s at mem pos %llu\n", world_name_char, objectNamePor);

		ULONG64 base = Memory::_This->Read_Memory(currentPlayer.Unknown + UNKNOWNENTITY_MODEL, _Test_ULONG64);
		ULONG64 model1 = Memory::_This->Read_Memory(base + MODEL_MODEL1, _Test_ULONG64);
		ULONG64 model2 = Memory::_This->Read_Memory(model1 + MODEL1_MODEL2, _Test_ULONG64);
		ULONG64 model3 = Memory::_This->Read_Memory(model2 + MODEL2_MODEL3, _Test_ULONG64);
		ULONG64 head = Memory::_This->Read_Memory(model3 + MODEL3_OFFSET + Bone_List::head * 8, _Test_ULONG64);
		//ULONG64 Chest = Memory::_This->Read_Memory(modelTransform + TRANSFORM_BONE + Bone_List::l_breast * 8, _Test_ULONG64);
		ULONG64 headWorld = Memory::_This->Read_Memory(head + BONE_WORLD, _Test_ULONG64);
		//ULONG64 ChestWorld = Memory::_This->Read_Memory(Chest + BONE_WORLD, _Test_ULONG64);

		//printf("\n before bone func \n");
		currentPlayer.head = headWorld;

		ULONG64 profile = Memory::_This->Read_Memory(currentPlayer.Unknown + UNKNOWNENTITY_PLAYERPROFILE, _Test_ULONG64);
		ULONG64 info = Memory::_This->Read_Memory(profile + PLAYERPROFILE_PLAYERINFO, _Test_ULONG64);

		ULONG64 playerNamePtr = Memory::_This->Read_Memory(info + PLAYERINFO_PLAYERNAME, _Test_ULONG64);
		wchar_t txt[64] = { 0 };
		Memory::_This->Read_Memory(playerNamePtr + 0x14, txt, 64);
		wstring ws(txt);
		// your new String
		string str(ws.begin(), ws.end());
		currentPlayer.name = str;
		currentPlayer.objName = "NPC";

		ULONG64 health = Memory::_This->Read_Memory(profile + PLAYERPROFILE_HEALTH, _Test_ULONG64);
		ULONG64 healthInfo = Memory::_This->Read_Memory(health + HEALTH_HEALTHINFO, _Test_ULONG64);
		currentPlayer.health = Memory::_This->Read_Memory(healthInfo + HEALTHINFO_CURRENT, _Test_ULONG64);

		//currentPlayer.position =Memory::_This->Read_Memory(currentPlayer.VisualState + VISUALSTATE_POSITION, _Test_Vector3);
		_Player_Vector.push_back(currentPlayer);
	}
	free(_Networked_List);
	//printf("%i", _Player_Vector.size());
	//printf("localplayer read");
}

//void Game_Data::Get_Data(bool box, bool animal, bool turret, bool vehicle)
//{
//	std::vector<Player> _Player_Vector_Temp;
//
//	ULONG64 ObjectManager = Memory::_This->Read_Memory(Memory::_This->Base + EFT_OBJECTMANAGER, _Test_ULONG64);
//	ULONG64 taggedObjectsPtr = Memory::_This->Read_Memory(ObjectManager + OBJECTMANAGER_TAGGEDLIST, _Test_ULONG64);
//	ULONG64 firstObject = Memory::_This->Read_Memory(ObjectManager + OBJECTMANAGER_TAGGEDLISTFIRST, _Test_ULONG64);
//
//	//walk the list
//	ULONG64 cycle = 0;
//	while (firstObject != taggedObjectsPtr)
//	{
//		cycle++;
//		Player currentPlayer = Player();
//		currentPlayer.BaseObject = taggedObjectsPtr;
//		currentPlayer.GameObject = Memory::_This->Read_Memory(taggedObjectsPtr + BASEENT_GAMEOBJECT, _Test_ULONG64);
//		currentPlayer.tag = Memory::_This->Read_Memory(currentPlayer.GameObject + GAMEOBJECT_TAG, _Test_INT16);
//
//		if (currentPlayer.tag == 5 || /*currentPlayer.tag == 20009 ||*/ currentPlayer.tag == 6)
//		{
//			//printf("\n gogogo %i \n", currentPlayer.tag);
//
//			if (currentPlayer.tag == 0)
//			{
//				//you fucked up somewhere
//				//printf("\nwalk list failed... tag 0 \n");
//				break;
//			}
//			if (currentPlayer.tag == 5)
//			{
//				//mainCamera
//				ULONG64 objClass = Memory::_This->Read_Memory(currentPlayer.GameObject + GAMEOBJECT_OBJECTCLASS, _Test_ULONG64);
//				ULONG64 entity = Memory::_This->Read_Memory(objClass + OBJECTCLASS_ENTITYPTR, _Test_ULONG64);
//				ULONG64 transform = Memory::_This->Read_Memory(objClass + OBJECTCLASS_TRANSFORM, _Test_ULONG64);
//
//				Game_Data::_Camera_Addr = entity;
//
//				currentPlayer.VisualState = Memory::_This->Read_Memory(transform + TRANSFORM_VISUALSTATE, _Test_ULONG64);
//			}
//			if (currentPlayer.tag == 20009)
//			{
//				//Corpse
//				ULONG64 objClass = Memory::_This->Read_Memory(currentPlayer.GameObject + GAMEOBJECT_OBJECTCLASS, _Test_ULONG64);
//				//ULONG64 entity =Memory::_This->Read_Memory(objClass + OBJECTCLASS_ENTITYPTR, _Test_ULONG64);
//				ULONG64 transform = Memory::_This->Read_Memory(objClass + OBJECTCLASS_TRANSFORM, _Test_ULONG64);
//
//				currentPlayer.VisualState = Memory::_This->Read_Memory(transform + TRANSFORM_VISUALSTATE, _Test_ULONG64);
//			}
//			//if (currentPlayer.tag == 20011) probably unnecessary
//			//{
//			//	//SkyDome
//			//	ULONG64 objClass =Memory::_This->Read_Memory(currentPlayer.GameObject + GAMEOBJECT_OBJECTCLASS, _Test_ULONG64);
//			//	//ULONG64 entity =Memory::_This->Read_Memory(objClass + OBJECTCLASS_ENTITYPTR, _Test_ULONG64);
//			//	ULONG64 transform =Memory::_This->Read_Memory(objClass + OBJECTCLASS_TRANSFORM, _Test_ULONG64);
//
//			//	currentPlayer.VisualState =Memory::_This->Read_Memory(transform + TRANSFORM_VISUALSTATE, _Test_ULONG64);
//			//}
//			if (currentPlayer.tag == 6)
//			{
//				firstObject = Memory::_This->Read_Memory(ObjectManager, _Test_ULONG64);
//				//Player
//				ULONG64 objClass = Memory::_This->Read_Memory(currentPlayer.GameObject + GAMEOBJECT_OBJECTCLASS, _Test_ULONG64);
//				ULONG64 entity = Memory::_This->Read_Memory(objClass + OBJECTCLASS_ENTITYPTR, _Test_ULONG64);
//				ULONG64 transform = Memory::_This->Read_Memory(objClass + OBJECTCLASS_TRANSFORM, _Test_ULONG64);
//
//				//printf("\n before local model \n");
//
//				//ULONG64 model =Memory::_This->Read_Memory(entity + ENTITYPTR_MODEL, _Test_ULONG64);
//				//ULONG64 modelTransform =Memory::_This->Read_Memory(model + MODEL_TRANSFORM, _Test_ULONG64);
//				//ULONG64 head =Memory::_This->Read_Memory(modelTransform + TRANSFORM_BONE + 47 * 8, _Test_ULONG64);
//				//ULONG64 headWorld =Memory::_This->Read_Memory(head + BONE_WORLD, _Test_ULONG64);
//
//				//printf("\n before bone func \n");
//				//currentPlayer.head = get_bone_pos(headWorld);
//				//printf("\n after bone func \n");
//				currentPlayer.VisualState = Memory::_This->Read_Memory(transform + TRANSFORM_VISUALSTATE, _Test_ULONG64);
//
//				ULONG64 objectNamePor = Memory::_This->Read_Memory(currentPlayer.GameObject + GAMEOBJECT_OBJECTNAMEPTR, _Test_ULONG64);
//				char world_name_char[100] = { 0 };
//				Memory::_This->Read_Memory(objectNamePor, world_name_char, 50);
//				//printf("The name is: %s at mem pos %llu\n", world_name_char, objectNamePor);
//				if (strstr(world_name_char, "LocalPlayer"))
//				{
//					//other stuff
//					//printf("\nlocal local at position %llu", cycle);
//					Game_Data::localplayer = currentPlayer;
//					currentPlayer.name = world_name_char;
//					currentPlayer.objName = "Localplayer";
//					ULONG64 base = Memory::_This->Read_Memory(entity + ENTITYPTR_BASEENT, _Test_ULONG64);
//					INT flag = Memory::_This->Read_Memory(base + BASEENT_FLAG, _Test_INT);
//					currentPlayer.flag = flag; // todo: fix this
//					//currentPlayer.position =Memory::_This->Read_Memory(currentPlayer.VisualState + VISUALSTATE_POSITION, _Test_Vector3);
//				}
//				else if (strstr(world_name_char, "npc"))
//				{
//					ULONG64 base = Memory::_This->Read_Memory(entity + ENTITYPTR_BASEENT, _Test_ULONG64);
//					INT flag = Memory::_This->Read_Memory(base + BASEENT_FLAG, _Test_INT);
//					currentPlayer.flag = flag;
//					ULONG64 model = Memory::_This->Read_Memory(base + BASEENT_MODEL, _Test_ULONG64);
//					ULONG64 modelTransform = Memory::_This->Read_Memory(model + MODEL_TRANSFORM, _Test_ULONG64);
//					ULONG64 head = Memory::_This->Read_Memory(modelTransform + TRANSFORM_BONE + 47 * 8, _Test_ULONG64);
//					ULONG64 headWorld = Memory::_This->Read_Memory(head + BONE_WORLD, _Test_ULONG64);
//
//					//printf("\n before bone func \n");
//					currentPlayer.head = headWorld;
//
//					//printf("\npeople people at position %llu", cycle);
//					ULONG64 playerNamePtr = Memory::_This->Read_Memory(base + BASEENT_NAME, _Test_ULONG64);
//					wchar_t txt[64] = { 0 };
//					Memory::_This->Read_Memory(playerNamePtr + 0x18, txt, 64);
//					wstring ws(txt);
//					// your new String
//					string str(ws.begin(), ws.end());
//					currentPlayer.name = str;
//
//					currentPlayer.objName = "NPC";
//
//					//currentPlayer.position =Memory::_This->Read_Memory(currentPlayer.VisualState + VISUALSTATE_POSITION, _Test_Vector3);
//				}
//				else
//				{
//					ULONG64 base = Memory::_This->Read_Memory(entity + ENTITYPTR_BASEENT, _Test_ULONG64);
//					INT flag = Memory::_This->Read_Memory(base + BASEENT_FLAG, _Test_INT);
//					currentPlayer.flag = flag;
//					ULONG64 model = Memory::_This->Read_Memory(base + BASEENT_MODEL, _Test_ULONG64);
//					ULONG64 modelTransform = Memory::_This->Read_Memory(model + MODEL_TRANSFORM, _Test_ULONG64);
//					ULONG64 head = Memory::_This->Read_Memory(modelTransform + TRANSFORM_BONE + 47 * 8, _Test_ULONG64);
//					ULONG64 headWorld = Memory::_This->Read_Memory(head + BONE_WORLD, _Test_ULONG64);
//
//					//printf("\n before bone func \n");
//					currentPlayer.head = headWorld;
//
//					//printf("\npeople people at position %llu", cycle);
//					ULONG64 playerNamePtr = Memory::_This->Read_Memory(base + BASEENT_NAME, _Test_ULONG64);
//					wchar_t txt[64] = { 0 };
//					Memory::_This->Read_Memory(playerNamePtr + 0x18, txt, 64);
//					wstring ws(txt);
//					// your new String
//					string str(ws.begin(), ws.end());
//					currentPlayer.name = str;
//
//					currentPlayer.objName = "Player";
//
//					//currentPlayer.position =Memory::_This->Read_Memory(currentPlayer.VisualState + VISUALSTATE_POSITION, _Test_Vector3);
//				}
//			}
//			_Player_Vector_Temp.push_back(currentPlayer);
//		}
//		taggedObjectsPtr = Memory::_This->Read_Memory(taggedObjectsPtr + TAGGEDLIST_NEXTTAGGEDENT, _Test_ULONG64);
//	}
//	//printf("inner cycle: %llu\n", cycle);
//	Game_Data::_Player_Vector = _Player_Vector_Temp;
//	return;
//}