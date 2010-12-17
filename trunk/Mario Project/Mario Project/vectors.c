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


#include "vectors.h"


vecti add_vecti(vecti v1, vecti v2)
{
  vecti res;
  
  res.x = v1.x + v2.x;
  res.y = v1.y + v2.y;

  return res;
}


vecti sub_vecti(vecti v1, vecti v2)
{
  vecti res;
  
  res.x = v1.x - v2.x;
  res.y = v1.y - v2.y;

  return res;
}


vecti mul_scal_vecti(vecti v1, int x)
{
  vecti res;
  
  res.x = v1.x * x;
  res.y = v1.y * x;

  return res;
}


unsigned int norme_vecti(vecti v)
{
  return (unsigned int)sqrt(v.x * v.x + v.y + v.y);
}


vecti *normalise_vecti(vecti *v)
{
  unsigned int norme;
  
  norme = norme_vecti(*v);

  v->x /= norme;
  v->y /= norme;

  return v;
}


float prod_scal_vecti(vecti v1, vecti v2)
{
  float angle = 0;
    
  /* calcul de l'angle */

  return norme_vecti(v1) * norme_vecti(v2) * (float)cos(angle);  
}



vectf add_vectf(vectf v1, vectf v2)
{
  vectf res;
  
  res.x = v1.x + v2.x;
  res.y = v1.y + v2.y;

  return res;
}


vectf sub_vectf(vectf v1, vectf v2)
{
  vectf res;
  
  res.x = v1.x - v2.x;
  res.y = v1.y - v2.y;

  return res;
}


vectf mul_scal_vectf(vectf v1, float x)
{
  vectf res;
  
  res.x = v1.x * x;
  res.y = v1.y * x;

  return res;
}


float norme_vectf(vectf v)
{
  return (float)sqrt(v.x * v.x + v.y + v.y);
}


vectf *normalise_vectf(vectf *v)
{
  float norme;
  
  norme = norme_vectf(*v);

  v->x /= norme;
  v->y /= norme;

  return v;
}


float prod_scal_vectf(vectf v1, vectf v2)
{
  float angle = 0;

  /* calcul de l'angle */

  return norme_vectf(v1) * norme_vectf(v2) * (float)cos(angle);  
}
