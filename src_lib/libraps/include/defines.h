/**
   \file defines.h
   \brief Описание основных структур, перечислений и инициализаций массивов.
   \author Кориков Сергей
   \version 0.1
   \date 2008-08-26
*/
#ifndef DEFINES_H
#define DEFINES_H

#include <string>


#define PORT_KSK = 50000


///номер пина счетчика для КСК
const int PIN_FREQ = 6;
/**
   \def dprintf
   \brief При отладке выводить сообщения на экран. При выключенной отладке - ничего не выводить.
*//**
#ifdef ENABLEDEBUG
#   define dprintf(fmt, args...) fprintf(stderr, fmt, ## args)
#else
#   define dprintf(fmt, args...)
#endif
*/
/**
   \def RU
   \brief Для удобства вывода на экран кириллицы в Qt-3.3.3.
*/
#define RU QString::fromLocal8Bit

/**
   \typedef ushort
   \brief Размер одного слова для кодограмм (беззнаковый короткий int, 2 байта).
*/
typedef unsigned short int ushort;

/**
   \typedef guchar
   \brief Беззнаковый char.
*/
typedef unsigned char guchar;

/** enum PROTOCOL_MARKERS
   \brief Виды маркеров, используемых в протоколе между ячейками и ЦУУ.
 */
enum PROTOCOL_MARKERS
{
     MNP = 0xFFF0, ///< Маркер начала пакета.
     MKP = 0xFFF8, ///< Маркер конца пакета.
     MKU = 0xFFFA  ///< Маркер команды.
};

enum
{
     RCVBUF_SIZE = 81 ///< Максимальное значение размера буфера для приема сообщений от корзин расширения.
};

/**
   \enum deviceType
   \brief Определение типов используемых аппаратов.
 */
enum ApparateType
{
     Argun = 10,   ///< Аргунь.
     Delta = 20,   ///< Дельта.
     Kulon = 30,   ///< Кулон.
     Serdolik = 40 ///< Сердолик.
};

/**
   \enum DeviceType
   \brief Тип опрашиваемого устройства.
*/
enum DeviceType
{
     Module, ///< Корзина расширения.
     Freq ///< Счетчик.
};

/**
   \enum InOut
   \brief Определение вспомогательных сокращений для ввода и вывода.
 */
enum InOut
{
     IN, ///< Ввод.
     OUT ///< Вывод.
};

/**
   \enum CHANGE_KEYS_ID
   \brief Варианты режима смены ключей. 
*/
enum CHANGE_KEYS_ID
{
     CHANGE_KEYS_WITHOUT_LSMENA = 1, ///< Без отложенной смены.
     CHANGE_KEYS_WITH_LSMENA         ///< С отложенной сменой.
};

/**
   \enum CALL_ID
   \brief Вызов и отбой.
*/
enum CALL_ID
{
     C_CALL = 2, ///< Вызов.
     C_DISCONNECT = 6 ///< Отбой.
};

/**
   \var const std::string DB_HOST
   \brief IP-адрес компьютера, где расположена БД.
*/
const std::string DB_HOST = "127.0.0.1";

/**
   \var const std::string CUU_HOST
   \brief IP-адрес ЦУУ.
*/
const std::string CUU_HOST = "192.168.1.1";

/**
   \var const std::string RMISH_HOST
   \brief IP-адрес РМИШ.
*/
const std::string RMISH_HOST = "192.168.1.9";

/**
   \struct kg
   \brief Полный состав кодограммы.
*/
struct kg
{
     ushort number;  ///< Номер комплекта.
     ushort command; ///< Действие.
     ushort param;   ///< Команда или другие дополнительные параметры.
};

/**
   \struct sa_info
   \brief Конфигурация спецаппарата.
*/
struct sa_info
{
     ushort num;   ///< Номер аппарата.
     ushort type;  ///< Тип аппарата.
     ushort nport; ///< Номер NPort сервера.
     ushort port;  ///< Номер порта для корзины расширения в NPort сервере.
     int sendbuf;     ///< Последние поданные команды.
};

/**
   \struct FullChanNum
   \brief Информация о положении спецаппарата в коммутационном оборудовании.
*/
struct FullChanNum
{
     ushort chan  : 4; ///< Канал.
     ushort cell  : 5; ///< Ячейка.
     ushort kreit : 2; ///< Крейт.
     ushort bit11 : 1; ///< Не используется.
     ushort ko    : 2; ///< Тип коммутационного оборудования.
     ushort bit14 : 1; ///< Не используется.
     ushort bit15 : 1; ///< Не используется.
};

/**
   \union chanNum
   \brief Представление FullChanNum как ushort.
*/
union chanNum
{
     ushort full;  ///< Представление FullChanNum как ushort.
     struct FullChanNum a; ///< \sa struct FullChanNum.
};

/**
   \struct MarkerInfo
   \brief Подробная информация для радиокакала.
*/
struct MarkerInfo
{
     ushort KO;      ///< Тип коммутационного оборудования.
     ushort KREIT;   ///< Крейт.
     ushort CELL;    ///< Ячейка.
     ushort CHAN;    ///< Канал.
     ushort KL;      ///< Поле ключа в маркере.
     ushort FNUM;    ///< Номер частоты.
     char   KIND[3]; ///< Тип системы связи: К-9 или К-4.
     char   MAR[4];  ///< Тип маркера.
};

/**
   \enum CMD_ID
   \brief Тип действия.
*/
enum CMD_ID
{
     CMD_ON       = 0x1,  ///< Включить в работу
     CMD_OFF      = 0x2,  ///< Отбой
     CMD_BLOCK    = 0x4,  ///< Заблокировать
     CMD_UNBLOCK  = 0x8,  ///< Разблокировать
     CMD_SET      = 0x10, ///< Взвести цепи
     CMD_UNSET    = 0x20, ///< Сбросить цепи
     CMD_CALL     = 0x40, ///< Для вызова и отбоя.
     CMD_NEXT_KEY = 0x80, ///< Следующий ключ
     CMD_LSMENA   = 0x100, ///< Отложенная смена
     CMD_PK       = 0x200, ///< Проверить тип вызова по ПК ( аппаратный или модемный)
     CMD_SM_PORT = 0x400 ///< Смена штерфейса для спец аппарата (номера порта в nport-е)
};

/**
   \enum APP_ID
   \brief Команда или другие дополнительные параметры.
 */
enum APP_ID
{
     APP_RABOTA      = 0x1,    ///< Работа.
     APP_PROVERKA    = 0x2,    ///< Проверка.
     APP_EZ          = 0x4,    ///< Эхо-заграждение.
     APP_PSH         = 0x8,    ///< ПШ.
     APP_K1          = 0x10,   ///< Установить К1.
     APP_K2          = 0x20,   ///< Установить К2.
     APP_NS          = 0x40,   ///< Гриф НС.
     APP_S           = 0x80,   ///< Ггриф С.
     APP_CALL_AUTO   = 0x100,  ///< ВЫЗ/ОТБ автоматический.
     APP_CALL_MANUAL = 0x200,  ///< ВЫЗ/ОТБ ручной.
     APP_ZSBA        = 0x400,  ///< ЗСБ-А.
     APP_POKK        = 0x800,  ///< ПОК-К.
     APP_POKA        = 0x1000, ///< ПОК-А.
     APP_VS          = 0x2000, ///< Выключение спецсигнализации (ВС).
     APP_RESET_KEYS  = 0x4000, ///< Сброс КИ.
};

/**
   \var const ushort cmds[5][16]
   \brief Таблица для перевода значений команд каждого типа аппарата
   \brief к универсальному APP-значению и наоборот.
   Значения команд хранятся в БД в виде APP-значений, соответствующих APP_ID.
   \sa enum APP_ID.
*/
const ushort cmds[5][16] =
{	
     // APP
     { 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080,
       0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000, 0x0000 },

     // Argun
     { 0x0020, 0x0008, 0x0010, 0x0040, 0x0080, 0x0100, 0x0001, 0x0002,
       0x0004, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },

     // Delta
     { 0x0040, 0x0001, 0x0002, 0x0080, 0x0010, 0x0008, 0x0000, 0x0004,
       0x0020, 0x0100, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },

     // Kulon
     { 0x0004, 0x0008, 0x0001, 0x0002, 0x0200, 0x0400, 0x0000, 0x0020, 
       0x0010, 0x0000, 0x1000, 0x0080, 0x0100, 0x2000, 0x4000, 0x0000 },

     // Serdolik
     //Работа.Проверка Эхо-з
     { 0x0001, 0x0002, 0x0004, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 } 
};

/**
   \enum IND_ID
   \brief Определение всех индикаций.
*/
enum IND_ID
{
     IND_PSBRK      = 0x1,    ///< Подтверждение сброса ключа
     IND_PVS        = 0x2,    ///< Подтверждение выключения спецсигнализации
     IND_NS         = 0x4,    ///< НС
     IND_S          = 0x8,    ///< С
     IND_AVARIA     = 0x10,   ///< Авария
     IND_PUSTK2     = 0x20,   ///< Подтверждение установки К2
     IND_PUSTK1     = 0x40,   ///< Подтверждение установки К1
     IND_CALL_AUTO  = 0x80,   ///< ВЫЗ/ОТБ автоматический
     IND_ISPRAVNO   = 0x100,  ///< Исправно
     IND_SN         = 0x200,  ///< Синхронизм
     IND_NALK2      = 0x400,  ///< Наличие К2
     IND_NALK1      = 0x800,  ///< Наличие К1
     IND_OK         = 0x1000, ///< Обрыв канала
     IND_NOKK       = 0x2000, ///< НОК-К
     IND_NOKA       = 0x4000, ///< НОК-А
     IND_NSD        = 0x8000  ///< НСД к УСО (1-"нет НСД", 0-"НСД")
};

/**
   \enum IND_KULON_ID
   \brief Определение индикаций специально для Кулона.
*/
enum IND_KULON_ID
{
     IND_NDKK = 0x400, ///< НДК-К
     IND_NDKA = 0x800, ///< НДК-А
};

/**
   \var const ushort inds[5][16]
   \brief Таблица для перевода значений индикаций каждого типа аппарата
   \brief к универсальному APP-значению и наоборот.
   Значения индикаций хранятся в БД в виде APP-значений, соответствующих IND_ID.
   \sa enum IND_ID.
 */
const ushort inds[5][16] =
{
     // APP
     { 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080,
       0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000, 0x8000 },

     // Argun
     { 0x0000, 0x0000, 0x0008, 0x0010, 0x0002, 0x0000, 0x0000, 0x0004,
       0x0000, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x8000 },

     // Delta
     { 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080,
       0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x0000, 0x0000, 0x8000 },

     // Kulon
     //Псбр    ПВС      НС     С       АВАР    ПУСТК1  ПУСТК2  ВЫЗ.ОТБ 
     { 0x4000, 0x2000, 0x0040, 0x0020, 0x0001, 0x0800, 0x0000, 0x0010,
       0x0002, 0x0004, 0x0080, 0x0200, 0x0008, 0x0100, 0x0400, 0x0000 },
     // ИСПР    СНХ    НК1     НК2      ОК     НОК-А   НОК-К	НСД к УСО 

     // Serdolik
     { 0x0000, 0x0000, 0x0000, 0x0000, 0x0001, 0x0000, 0x0000, 0x0000,
       0x0002, 0x0008, 0x0000, 0x0000, 0x0004, 0x0000, 0x0000, 0x0010 }
};

#endif
