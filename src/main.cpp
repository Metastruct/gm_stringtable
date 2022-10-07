#undef _UNICODE

#include "filesystem.h"

#ifdef _WIN32
#include <windows.h>
#endif

#include <GarrysMod/Lua/Interface.h>
#include <GarrysMod/InterfacePointers.hpp>

#include "networkstringtabledefs.h"
#include "mathlib/mathlib.h"

#undef min 
#undef max
#include <string>
#include <sstream>

#define STRINGTABLE_NAME "StringTable"

INetworkStringTableContainer* g_NetworkStringTable = NULL;
int stringTableId = GarrysMod::Lua::Type::NONE;

LUA_FUNCTION(Dump)
{
	LUA->CheckType(1, stringTableId);

	INetworkStringTable* netTbl = LUA->GetUserType<INetworkStringTable>(1, stringTableId);

	if (netTbl) {
#ifdef CUSTOM_STRINGTABLE_HEADER	
		netTbl->Dump();
#endif
	}
	else
		LUA->ThrowError("gm_stringtables: Invalid StringTable!\n");

	return 0;
}

LUA_FUNCTION(Lock)
{
	LUA->CheckType(1, stringTableId);
	LUA->CheckType(2, GarrysMod::Lua::Type::Bool);

	INetworkStringTable* netTbl = LUA->GetUserType<INetworkStringTable>(1, stringTableId);

	if (netTbl) {
#ifdef CUSTOM_STRINGTABLE_HEADER		
		netTbl->Lock(LUA->GetBool(2));
#endif
	}
	else
		LUA->ThrowError("gm_stringtables: Invalid StringTable!\n");

	return 0;
}

LUA_FUNCTION(GetTableNames)
{
	LUA->CreateTable();

	for (int i = 0; i < g_NetworkStringTable->GetNumTables(); i++)
	{
		LUA->PushNumber(i);
		LUA->PushString(g_NetworkStringTable->GetTable(i)->GetTableName());
		LUA->SetTable(-3);
	}

	return 1;
}

LUA_FUNCTION(GetTable)
{
	LUA->CheckType(1, stringTableId);

	INetworkStringTable* netTbl = LUA->GetUserType<INetworkStringTable>(1, stringTableId);

	if (netTbl) {
		LUA->CreateTable();

		for (int i = 0; i < netTbl->GetNumStrings(); i++) {
			int b;
			LUA->PushString((const char*)netTbl->GetStringUserData(i, &b));
			LUA->SetField(-2, netTbl->GetString(i));
		}

		return 1;
	}
	else
		LUA->ThrowError("gm_stringtables: Invalid StringTable!\n");

	return 0;
}

LUA_FUNCTION(GetTableStrings)
{
	LUA->CheckType(1, stringTableId);

	INetworkStringTable* netTbl = LUA->GetUserType<INetworkStringTable>(1, stringTableId);

	if (netTbl) {
		LUA->CreateTable();

		for (int i = 0; i < netTbl->GetNumStrings(); i++) {
			LUA->PushNumber(i);
			LUA->PushString(netTbl->GetString(i));
			LUA->SetTable(-3);
		}

		return 1;
	}
	else
		LUA->ThrowError("gm_stringtables: Invalid StringTable!\n");

	return 0;
}


LUA_FUNCTION(GetTableData)
{
	LUA->CheckType(1, stringTableId);

	INetworkStringTable* netTbl = LUA->GetUserType<INetworkStringTable>(1, stringTableId);

	if (netTbl) {
		LUA->CreateTable();

		for (int i = 0; i < netTbl->GetNumStrings(); i++) {
			int len;
			const char* userData = (const char*)netTbl->GetStringUserData(i, &len);

			LUA->PushNumber(i);
			LUA->PushString(userData, len);
			LUA->SetTable(-3);
		}

		return 1;
	}
	else
		LUA->ThrowError("gm_stringtables: Invalid StringTable!\n");

	return 0;
}

LUA_FUNCTION(GetName) 
{
	LUA->CheckType(1, stringTableId);
	INetworkStringTable* netTbl = LUA->GetUserType<INetworkStringTable>(1, stringTableId);

	if (netTbl) {
		LUA->PushString(netTbl->GetTableName());
		return 1;
	}
	else
		LUA->ThrowError("gm_stringtables: Invalid StringTable!\n");

	return 0;
}

LUA_FUNCTION(GetTableId)
{
	LUA->CheckType(1, stringTableId);
	INetworkStringTable* netTbl = LUA->GetUserType<INetworkStringTable>(1, stringTableId);

	if (netTbl) {
		LUA->PushNumber((float)netTbl->GetTableId());
		return 1;
	}
	else
		LUA->ThrowError("gm_stringtables: Invalid StringTable!\n");

	return 0;
}

LUA_FUNCTION(GetNumStrings) 
{
	LUA->CheckType(1, stringTableId);
	INetworkStringTable* netTbl = LUA->GetUserType<INetworkStringTable>(1, stringTableId);

	if (netTbl) {
		LUA->PushNumber((float)netTbl->GetNumStrings());
		return 1;
	}
	else
		LUA->ThrowError("gm_stringtables: Invalid StringTable!\n");

	return 0;
}

LUA_FUNCTION(GetMaxStrings) 
{
	LUA->CheckType(1, stringTableId);
	INetworkStringTable* netTbl = LUA->GetUserType<INetworkStringTable>(1, stringTableId);

	if (netTbl) {
		LUA->PushNumber((float)netTbl->GetMaxStrings());
		return 1;
	}
	else
		LUA->ThrowError("gm_stringtables: Invalid StringTable!\n");

	return 0;
}

LUA_FUNCTION(GetEntryBits) 
{
	LUA->CheckType(1, stringTableId);
	INetworkStringTable* netTbl = LUA->GetUserType<INetworkStringTable>(1, stringTableId);

	if (netTbl) {
		LUA->PushNumber((float)netTbl->GetEntryBits());
		return 1;
	}
	else
		LUA->ThrowError("gm_stringtables: Invalid StringTable!\n");

	return 0;
}

LUA_FUNCTION(SetTick) 
{
	LUA->CheckType(1, stringTableId);
	LUA->CheckType(2, GarrysMod::Lua::Type::NUMBER);

	INetworkStringTable* netTbl = LUA->GetUserType<INetworkStringTable>(1, stringTableId);

	if (netTbl)
		netTbl->SetTick(LUA->GetNumber(2));
	else
		LUA->ThrowError("gm_stringtables: Invalid StringTable!\n");

	return 0;
}

LUA_FUNCTION(ChangedSinceTick) 
{

	LUA->CheckType(1, stringTableId);
	LUA->CheckType(2, GarrysMod::Lua::Type::NUMBER);

	INetworkStringTable* netTbl = LUA->GetUserType<INetworkStringTable>(1, stringTableId);

	if (netTbl) {
		LUA->PushNumber(netTbl->ChangedSinceTick(LUA->GetNumber(2)));
		return 1;
	}
	else
		LUA->ThrowError("gm_stringtables: Invalid StringTable!\n");

	return 0;
}

LUA_FUNCTION(AddString) 
{

	LUA->CheckType(1, stringTableId);
	LUA->CheckType(2, GarrysMod::Lua::Type::BOOL);
	LUA->CheckType(3, GarrysMod::Lua::Type::STRING);

	INetworkStringTable* netTbl = LUA->GetUserType<INetworkStringTable>(1, stringTableId);

	if (netTbl)
		netTbl->AddString(LUA->GetBool(2), LUA->GetString(3));
	else
		LUA->ThrowError("gm_stringtables: Invalid StringTable!\n");

	return 0;
}

LUA_FUNCTION(SetData) 
{

	LUA->CheckType(1, stringTableId);
	LUA->CheckType(2, GarrysMod::Lua::Type::NUMBER);
	LUA->CheckType(3, GarrysMod::Lua::Type::STRING);

	INetworkStringTable* netTbl = LUA->GetUserType<INetworkStringTable>(1, stringTableId);


	// WTF IS THIS UGLYNESS
	if (netTbl) {
		netTbl->SetStringUserData(LUA->GetNumber(2), strlen(LUA->GetString(3)) + 1, LUA->GetString(3));
	}
	else
		LUA->ThrowError("gm_stringtables: Invalid StringTable!\n");

	return 0;
}

LUA_FUNCTION(GetString) 
{

	LUA->CheckType(1, stringTableId);
	LUA->CheckType(2, GarrysMod::Lua::Type::NUMBER);

	INetworkStringTable* netTbl = LUA->GetUserType<INetworkStringTable>(1, stringTableId);

	if (netTbl) {
		int b;
		const char* strptr = (const char*)netTbl->GetString(LUA->GetNumber(2));
		if (strptr) {
			LUA->PushString(strptr);
			LUA->PushString((const char*)netTbl->GetStringUserData(LUA->GetNumber(2), &b));
			return 2;
		}
	}
	else
		LUA->ThrowError("gm_stringtables: Invalid StringTable!\n");

	return 0;
}


LUA_FUNCTION(GetData) 
{

	LUA->CheckType(1, stringTableId);
	LUA->CheckType(2, GarrysMod::Lua::Type::NUMBER);

	INetworkStringTable* netTbl = LUA->GetUserType<INetworkStringTable>(1, stringTableId);

	if (netTbl) {
		int datalen;
		const char* val = (const char*)netTbl->GetStringUserData(LUA->GetNumber(2), &datalen);
		if (val && datalen > 0 && datalen < 0x1fffff00) {
			LUA->PushString(val, datalen);
			LUA->PushNumber((double)datalen);
			return 2;
		}
	}
	else
		LUA->ThrowError("gm_stringtables: Invalid StringTable!\n");

	return 0;
}


LUA_FUNCTION(GetBool) 
{

	LUA->CheckType(1, stringTableId);
	LUA->CheckType(2, GarrysMod::Lua::Type::NUMBER);

	INetworkStringTable* netTbl = LUA->GetUserType<INetworkStringTable>(1, stringTableId);

	if (netTbl) {
		int b;
		const long thing = (long)netTbl->GetStringUserData(LUA->GetNumber(2), &b);
		LUA->PushNumber(thing == 1 ? true : false);
		return 1;
	}
	else
		LUA->ThrowError("gm_stringtables: Invalid StringTable!\n");

	return 0;
}

LUA_FUNCTION(GetNumber) 
{

	LUA->CheckType(1, stringTableId);
	LUA->CheckType(2, GarrysMod::Lua::Type::NUMBER);

	INetworkStringTable* netTbl = LUA->GetUserType<INetworkStringTable>(1, stringTableId);

	if (netTbl) {
		int b;
		const long thing = (long)netTbl->GetStringUserData(LUA->GetNumber(2), &b);
		LUA->PushNumber((float)thing);
		return 1;
	}
	else
		LUA->ThrowError("gm_stringtables: Invalid StringTable!\n");

	return 0;
}

/*LUA_FUNCTION( SetBool ) {

	LUA->CheckType( 1, stringTableId );
	LUA->CheckType( 2, Type::NUMBER );
	LUA->CheckType( 3, Type::BOOL );

	INetworkStringTable *netTbl = ( INetworkStringTable* ) LUA->GetUserdata(1);

	if ( netTbl ) {
		netTbl->SetStringUserData( LUA->GetInteger(2), strlen( LUA->GetString(3) ) + 1, (bool*) LUA->GetBool(3) );
	} else
		LUA->ThrowError("gm_stringtables: Invalid StringTable!\n");

	return 0;
}*/

LUA_FUNCTION(FindStringIndex) 
{

	LUA->CheckType(1, stringTableId);
	LUA->CheckType(2, GarrysMod::Lua::Type::STRING);

	INetworkStringTable* netTbl = LUA->GetUserType<INetworkStringTable>(1, stringTableId);

	if (netTbl) {
		LUA->PushNumber((float)netTbl->FindStringIndex(LUA->GetString(2)));
		return 1;
	}
	else
		LUA->ThrowError("gm_stringtables: Invalid StringTable!\n");

	return 0;
}

LUA_FUNCTION(SetAllowClientSideAddString) 
{

	LUA->CheckType(1, stringTableId);
	LUA->CheckType(2, GarrysMod::Lua::Type::BOOL);

	INetworkStringTable* netTbl = LUA->GetUserType<INetworkStringTable>(1, stringTableId);

	if (netTbl)
		g_NetworkStringTable->SetAllowClientSideAddString(netTbl, LUA->GetBool(2));
	else
		LUA->ThrowError("gm_stringtables: Invalid StringTable!\n");

	return 0;
}

LUA_FUNCTION(__eq)
{
	LUA->CheckType(1, stringTableId);
	LUA->CheckType(2, stringTableId);

	INetworkStringTable* stringTable1 = LUA->GetUserType<INetworkStringTable>(1, stringTableId);
	INetworkStringTable* stringTable2 = LUA->GetUserType<INetworkStringTable>(2, stringTableId);

	if (stringTable1 == stringTable2)
		LUA->PushBool(true);
	else
		LUA->PushBool(false);

	return 1;
}

// this is from backwards_headers, not sure why it was needed but here you go
void PushVA(GarrysMod::Lua::ILuaBase* lua, const char* str, ...)
{
	char buff[1024];
	va_list argptr;
	va_start(argptr, str);
#ifdef _WIN32
	vsprintf_s(buff, str, argptr);
#else
	vsprintf(buff, str, argptr);
#endif
	va_end(argptr);
	lua->PushString(buff);
}

LUA_FUNCTION(__tostring)
{
	LUA->CheckType(1, stringTableId);

	INetworkStringTable* stringTable = LUA->GetUserType<INetworkStringTable>(1, stringTableId);

	if (!stringTable)
	{
		LUA->ThrowError("gm_stringtables: Invalid StringTable!\n");
		return 0;
	}

	PushVA(LUA, "%s: %p", STRINGTABLE_NAME, stringTable);
	return 1;
}

LUA_FUNCTION(__new)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::STRING);

	INetworkStringTable* stringTable = g_NetworkStringTable->FindTable(LUA->GetString(1));
	if (!stringTable)
	{
		LUA->ThrowError("gm_stringtables: Invalid StringTable!\n");
		return 0;
	}

	LUA->PushUserType(stringTable, stringTableId);
	return 1;
}

void CreateMetaTable(GarrysMod::Lua::ILuaBase* lua) {
	stringTableId = lua->CreateMetaTable(STRINGTABLE_NAME);

	lua->PushCFunction(Dump);
	lua->SetField(-2, "Dump");

	lua->PushCFunction(Lock);
	lua->SetField(-2, "Lock");

	lua->PushCFunction(GetTable);
	lua->SetField(-2, "GetTable");

	lua->PushCFunction(GetTableStrings);
	lua->SetField(-2, "GetTableStrings");

	lua->PushCFunction(GetTableStrings);
	lua->SetField(-2, "GetTableStrings");

	lua->PushCFunction(GetTableData);
	lua->SetField(-2, "GetTableData");

	lua->PushCFunction(GetName);
	lua->SetField(-2, "GetName");

	lua->PushCFunction(GetTableId);
	lua->SetField(-2, "GetTableId");

	lua->PushCFunction(GetNumStrings);
	lua->SetField(-2, "GetNumStrings");

	lua->PushCFunction(GetMaxStrings);
	lua->SetField(-2, "GetMaxStrings");

	lua->PushCFunction(GetEntryBits);
	lua->SetField(-2, "GetEntryBits");

	lua->PushCFunction(SetTick);
	lua->SetField(-2, "SetTick");

	lua->PushCFunction(ChangedSinceTick);
	lua->SetField(-2, "ChangedSinceTick");

	lua->PushCFunction(AddString);
	lua->SetField(-2, "AddString");

	lua->PushCFunction(SetData);
	lua->SetField(-2, "SetData");

	lua->PushCFunction(GetString);
	lua->SetField(-2, "GetString");

	lua->PushCFunction(GetData);
	lua->SetField(-2, "GetData");

	lua->PushCFunction(GetBool);
	lua->SetField(-2, "GetBool");

	lua->PushCFunction(GetNumber);
	lua->SetField(-2, "GetNumber");

	lua->PushCFunction(FindStringIndex);
	lua->SetField(-2, "FindStringIndex");

	lua->PushCFunction(SetAllowClientSideAddString);
	lua->SetField(-2, "SetAllowClientSideAddString");

	lua->PushCFunction(__tostring);
	lua->SetField(-2, "__tostring");

	lua->PushCFunction(__eq);
	lua->SetField(-2, "__eq");

	lua->PushMetaTable(stringTableId);
	lua->SetField(-2, "__index");

	lua->Pop(1);
}

GMOD_MODULE_OPEN()
{
	//SourceSDK::FactoryLoader engine_loader("engine");
	//auto a = engine_loader.GetInterface<INetworkStringTableContainer>(networkstringtableclient_name);
	g_NetworkStringTable = InterfacePointers::NetworkStringTableContainer();

	if (g_NetworkStringTable == nullptr)
	{
		LUA->ThrowError("gm_stringtables: Error getting INetworkStringTableContainer interface.\n");
		return 0;
	}

	LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);

		LUA->PushCFunction(GetTableNames);
		LUA->SetField(-2, "GetAllStringTables");

		LUA->PushCFunction(__new);
		LUA->SetField(-2, "StringTable");

	LUA->Pop(1);

	CreateMetaTable(LUA);

	return 0;
}

GMOD_MODULE_CLOSE()
{
	return 0;
}
