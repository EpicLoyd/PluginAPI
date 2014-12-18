
//#include "qcommon/qcommon.h"
#include "qcommon\q_shared.h"
#include "TestPlugin.h"
///////////////////////////////COMMANDS

pluginImport_t *trap = NULL;


int Q_stricmpn(const char *s1, const char *s2, int n) {
	int		c1, c2;

	// bk001129 - moved in 1.17 fix not in id codebase
	if (s1 == NULL) {
		if (s2 == NULL)
			return 0;
		else
			return -1;
	}
	else if (s2 == NULL)
		return 1;



	do {
		c1 = *s1++;
		c2 = *s2++;

		if (!n--) {
			return 0;		// strings are equal until end point
		}

		if (c1 != c2) {
			if (c1 >= 'a' && c1 <= 'z') {
				c1 -= ('a' - 'A');
			}
			if (c2 >= 'a' && c2 <= 'z') {
				c2 -= ('a' - 'A');
			}
			if (c1 != c2) {
				return c1 < c2 ? -1 : 1;
			}
		}
	} while (c1);

	return 0;		// strings are equal
}

int Q_stricmp(const char *s1, const char *s2) {
	return (s1 && s2) ? Q_stricmpn(s1, s2, 99999) : -1;
}

typedef struct svcmd_s {
	const char	*name;
	void(*func)(void);
	qboolean	dedicated;
} svcmd_t;

void TestFunction(void){

	Com_Printf("TESTING");
}

int svcmdcmp(const void *a, const void *b) {
	return Q_stricmp((const char *)a, ((svcmd_t*)b)->name);
}


/* This array MUST be sorted correctly by alphabetical name field */
svcmd_t commands[] = {
	{ "testplugin", TestFunction},
};
static const size_t numCommands = ARRAY_LEN(commands);

qboolean ConsoleCommand(void) {
	char	cmd[MAX_TOKEN_CHARS] = { 0 };
	svcmd_t	*command = NULL;

	trap->Argv(0, cmd, sizeof(cmd));

	command = (svcmd_t *)bsearch(cmd, commands, numCommands, sizeof(commands[0]), svcmdcmp);
	if (!command)
		return qfalse;

	command->func();
	return qtrue;
}




//////////////////////////////////

plugininfo_t *G_InitPlugin(int levelTime){
	//trap->AddCommand("testplugin", TestFunction);
}
void  G_ShutdownPlugin(int restart){
	trap->Print("Shutdown\n");
}

char *ClientConnect(int clientNum, qboolean firstTime, qboolean isBot){
	trap->Print("CLIENT CONNECTING....\n");
	return NULL;
}

int  ClientBegin(int clientNum, qboolean allowTeamReset){
	trap->Print("CLIENT BEGIN\n");
}

qboolean *ClientUserinfoChanged(int clientNum){
	trap->Print("USER INFO CHANGED\n");
	return qtrue;
}

void    *ClientDisconnect(int clientNum){
	trap->Print("CLIENT DISCONNECT\n");
}

int   *ClientCommand(int clientNum){
	trap->Print("Client Command\n");
}

void    *ClientThink(int clientNum, usercmd_t *ucmd) {
	//trap->Print("Client Think\n");
}

void   *RunFrame(int levelTime) {
	//trap->Print("Frame");
}


void QDECL CallFunction(int cmd, int numargs, va_list *args);

Q_EXPORT plugininfo_t* QDECL GetPluginAPI(int apiVersion, pluginImport_t *import)
	{

	    plugininfo_t *ge = (plugininfo_t*)malloc(sizeof(plugininfo_t));

		ge->name = "Plugin for SABER SERVER";
		ge->author = "EpicLoyd";
		ge->version = "1.0";
		ge->usegamelibrary = qtrue;

		assert(import);
		trap = import;
		Com_Printf = trap->Print;
		Com_Error = trap->Error;

		ge->export = (pluginExport_t*)malloc(sizeof(pluginExport_t));
		if (apiVersion != PLUGIN_API_VERSION) {
			trap->Print("Mismatched PLUGIN_API_VERSION: expected %i, got %i\n", PLUGIN_API_VERSION, apiVersion);
			return NULL;
		}

		ge->export->InitPlugin = G_InitPlugin;
		ge->export->ShutDownPlugin = G_ShutdownPlugin;
		ge->export->ClientConnect = ClientConnect;
		ge->export->ClientBegin = ClientBegin;
		ge->export->ClientCommand = ClientCommand;
		ge->export->ClientDisconnect = ClientDisconnect;
		ge->export->ClientThink = ClientThink;
		ge->export->ClientUserinfoChanged = ClientUserinfoChanged;
		ge->export->ConsoleCommand = ConsoleCommand;
		ge->export->RunFrame = RunFrame;
		ge->export->Call = CallFunction;

		return ge;
	}

void QDECL CallFunction(int cmd, int numargs, va_list *args){
	trap->Print("TEST SECONDARY ENTRY argsnum: %d\n", numargs);
	char *nigra = NULL;

	for (int i = 0; i < numargs; i++){
     nigra = va_arg(*args, char*);
	 Com_Printf("AJAJJAJAJ: %s \n", nigra);
}

}