#pragma once
#include"预编译头.h"

using namespace std;
enum landType : int {
	PUTBLOCK = 1,
	DESTROYBLOCK = 2,
	USEITEM = 3,
	OPENCHEST = 4,
	ATTACKT = 5,
};
enum whicthType : int {
	PBUG = 0,
	P1111 = 1,
	P1110 = 2,
	P1101 = 3,
	P1100 = 4,
	P1011 = 5,
	P1010 = 6,
	P1001 = 7,
	P1000 = 8,
	P0111 = 9,
	P0110 = 10,
	P0101 = 11,
	P0100 = 12,
	P0011 = 13,
	P0010 = 14,
	P0001 = 15,
	P0000 = 16,
	lastppp = 17
};
//操作配置文件
string getjson(string name);
void writeLandJson(string uid, string po, string name);
void removeLandJson(string point);
void writeConfigJson();
void writePlayerJson(string uid, string land, string share_name, bool putblock, bool destroyblock, bool useitem, bool openchest, bool attack);
//void setPlayerJson(string uid, string share_name, bool putblock, bool destroyblock, bool useitem, bool openchest, bool attack);
void removePlayerJson_land_sharename(string uid, string land, string sharename);

//领地判断
string island(int x, int y, int z);
string checkland(int xa, int ya, int za, int xb, int yb, int zb);
bool island_player(string uuid, string name, string point/*x.y.z*/, landType id);
bool land_is(int x, int y, int z, int x1, int y1, int z1, int x2, int y2, int z2);
bool land_buy(string uid, int* s, int* money);
bool land_sell(string uid, string point, int* money, string* landpo);

//调用
whicthType use_what_byxz(int x1, int z1, int x2, int z2);
string island_circle(string po, int x, int y, int z);
string get_land_json(string point, string id);
int get_maxsize();
int get_maxland();
string get_setmoney();
int get_landmoney(bool a);
int get_player_land_num(string uid);
void get_player_json(string uid, bool* putblock, bool* destroyblock, bool* useitem, bool* openchest, bool* attack);

//经济模块
int getmoney(string uuid);
void addmoney(string uuid, int money);
bool removemoney(string uuid, int money);

//gui
void sendText(string uuid, string text, TextType tp);
extern unsigned sendForm(std::string, std::string);
std::string createModalFormString(std::string title, std::string content, std::string button1, std::string button2);
extern bool destroyForm(unsigned);
static UINT send_landbuygui(string uuid, string title, string content, string button1, string button2);
static UINT landhelp_gui(std::string uuid);
static UINT send_landsellgui(string uuid, string title, string content, string button1, string button2);
void send_landinfo_gui(string uuid, string point,string landplayeruuid, string name, string share_name);
void land_info(string uid, string point);
static UINT send_landshare_gui(string uuid);
static UINT send_landpermission_gui(string uuid);