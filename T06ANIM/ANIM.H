/* FILENAME: ANIM.H
 * PROGRAMMER: DS6
 * PURPOSE: Animation system declaration module.
 * LAST UPDATE: 08.06.2015
 */

#ifndef __ANIM_H_
#define __ANIM_H_

#include "def.h"
#include "render.h"

/* Максимальное количество элементов анимации */
#define DS6_MAX_UNITS 3000

/* Объявление типа объектов анимации "вперед" */
typedef struct tagds6UNIT ds6UNIT;

/* Структура хранения контекста анимации */
typedef struct
{
  HWND hWnd;          /* Окно вывода */
  INT W, H;           /* Размер окна вывода */
  HBITMAP hBmFrame;   /* Буфер кадра */
  HDC hDC;            /* Контекст вывода */
  BOOL IsPause;       /* Проверка на паузу */
  DBL GlobalTime, 
      GlobalDeltaTime, 
      DeltaTime, 
      Time, 
      FPS;
  /* Подсистема ввода */
  BYTE
    Keys[256],       /* Состояние клавиш клавиатуры и мыши */
    KeysOld[256],    /* Состояние клавиш на предыдущем кадре */
    KeysClick[256];  /* Флаги однократного нажатия */
  INT
    MsDeltaX, MsDeltaY, /* Относительные координаты курсора мыши */
    MsX, MsY,        /* Абсолютные координаты курсора мыши */
    MsWheel;         /* Состояние колеса мыши */
  BYTE
    JButs[32],       /* Состояние кнопок джойстика */
    JButsOld[32],    /* Состояние кнопок джойстика на предыдущем кадре */
    JButsClick[32],  /* Флаги однократного нажатия кнопок джойстика */
    JPOV;            /* Переключатель POV - 0..8 */
  DBL
    JX, JY, JZ, JR, JU, JV; /* Оси джойстика (-1.0 .. 1.0*/

  /* Массив элементов анимации и их количество */
  ds6UNIT *Units[DS6_MAX_UNITS]; 
  INT NumOfUnits;

} ds6ANIM;

extern ds6ANIM DS6_Anim;

/* Базовые поля объекта анимации:
 * - размер структуры для наследования
 *     INT Size;
 * - идентификатор объеткта:
 *     INT Id;
 * - указатель на функцию инициализации
 *     VOID (*Init)( ds6UNIT *Unit, ds6ANIM *Ani );
 * - указатель на функцию деинициализации
 *     VOID (*Close)( ds6UNIT *Unit, ds6ANIM *Ani );
 * - указатель на функцию обновления межкадровых параметров
 *     VOID (*Response)( ds6UNIT *Unit, ds6ANIM *Ani );
 * - указатель на функцию построения
 *     VOID (*Render)( ds6UNIT *Unit, ds6ANIM *Ani );
 */
#define DS6_UNIT_BASE_FIELDS \
  INT Size;                                          \
  VOID (*Init)( ds6UNIT *Unit, ds6ANIM *Ani );       \
  VOID (*Close)( ds6UNIT *Unit, ds6ANIM *Ani );      \
  VOID (*Response)( ds6UNIT *Unit, ds6ANIM *Ani );   \
  VOID (*Render)( ds6UNIT *Unit, ds6ANIM *Ani )

/* Базовый тип объекта анимации */
struct tagds6UNIT
{
  DS6_UNIT_BASE_FIELDS;
};

/* Функция инициализации анимации.
 * АРГУМЕНТЫ:
 *   - дескриптор окна:
 *       HWND hWnd;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID DS6_AnimInit( HWND hWnd );

/* Функция деинициализации анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID DS6_AnimClose( VOID );

/* Функция обработки изменения размера области вывода.
 * АРГУМЕНТЫ:
 *   - новый размер области вывода:
 *       INT W, H;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID DS6_AnimResize( INT W, INT H );

/* Функция построения кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID DS6_AnimRender( VOID );

/* Функция вывода кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID DS6_AnimCopyFrame( VOID );

/* Функция добавления в систему объекта анимации.
 * АРГУМЕНТЫ:
 *   - добавляемый объект анимации:
 *       ds6UNIT *Unit;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID DS6_AnimAddUnit( ds6UNIT *Unit );

/* Функция переключения в/из полноэкранного режима
 * с учетом нескольких мониторов.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID DS6_AnimFlipFullScreen( VOID );

/* Функция создания объекта анимации.
 * АРГУМЕНТЫ:
 *   - размер структуры объекта анимации:
 *       INT Size;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (ds6UNIT *) указатель на созданный объект анимации.
 */
ds6UNIT * DS6_AnimUnitCreate( INT Size );

/* Функция выхода из анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID DS6_AnimDoExit( VOID );

/* Set pause function.
 * ARGUMENTS: 
 *   - pause flag:
 *       BOOL NewPAuseFlag;
 * RETURNS: none.
 */  
VOID DS6_AnimSetPause( BOOL NewPauseFlag );

extern INT DS6_MouseWheel;


#endif /* __ANIM_H_ */

/* END OF 'ANIM.H' FILE */