/*============================================================================*/
/*== [sound.c] [Version: 0.028a]                                            ==*/
/*============================================================================*/
/*===         _____         _        _____           _         _           ===*/
/*===        |     |___ ___|_|___   |  _  |___ ___  |_|___ ___| |_         ===*/
/*===        | | | | .'|  _| | . |  |   __|  _| . | | | -_|  _|  _|        ===*/
/*===        |_|_|_|__,|_| |_|___|  |__|  |_| |___|_| |___|___|_|          ===*/
/*===                                             |___|                    ===*/
/*============================================================================*/
/*===      =LagMan=                =McCaml=                 =MouleMan=     ===*/
/*============================================================================*/

/******************************************************************************/
/*=================================[INCLUDES]=================================*/
/******************************************************************************/


#include "sound.h"


FSOUND_SAMPLE* charger_son(char *nom, int volume)
{
  FSOUND_SAMPLE *son = NULL;

  son = FSOUND_Sample_Load(FSOUND_FREE, nom, 0, 0, 0);
  FSOUND_SetVolume(FSOUND_FREE, volume);

  return son;
}


FSOUND_STREAM* charger_musique(char* nom, int volume, int isloop)
{
	char nom_musique[TAILLE_TITRE_MUSIQUE] = {'\0'};
	FSOUND_STREAM *zik = NULL;

	strcpy(nom_musique, "musics/");
	strcat(nom_musique, nom);

	zik = FSOUND_Stream_Open(nom_musique, FSOUND_LOOP_NORMAL, 0, 0);
	FSOUND_SetVolume(FSOUND_FREE, volume);

	if(isloop)
		FSOUND_Stream_SetLoopCount(zik, -1);

	return zik;
}
