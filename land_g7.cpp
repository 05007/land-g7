#include "Ԥ����ͷ.h"
#include "�ṹ��.h"
#include "Form.h"
#include "json_tool.h"
#include "command.h"
#include "land_g7.h"
#include <functional>
#include <queue>
#pragma warning(disable : 4996)
using namespace std;

static std::map<string, bool> playerOpen;//����Ƿ�����ѡ��ģʽ
static std::map<std::string, Player*> onlinePlayers;//�����б�
static std::map<string, string> playerName;
static std::map<Player*, bool> player_isoline;

static std::map<std::string,std::string> playerSetA;//����Ƿ������˵�a
static std::map<std::string,std::string> playerSetB;

static std::map<unsigned, bool> fids;
static std::map<std::string, UINT> gui_buyfid;//��fid����
static std::map<std::string, UINT> gui_helpfid;
static std::map<std::string, UINT> gui_sellfid;
static std::map<std::string, UINT> gui_landsharefid;
static std::map<std::string, UINT> gui_landpermissionfid;

static std::map<std::string,std::string> gui_land_sharename;//���������

string LANDCONFIG;
string LCONFIG;
string PLAYERCONFIG;

static std::map<int, bool> debug_useitem_open;//������������������Χһ�����޷�ʹ����Ʒ

//�������
void init() {
	_mkdir("land");//�����ļ���
    ifstream File;
	cout << u8"��ز��(land-g7.dll)�Ѿ����ء�\n�汾��0.0.1-���԰汾��" << endl;
	cout << u8"����������ӵ����һ���汾��\n���ܻ���Զ����������С�" << endl;
	cout << u8"�������ȫ��ѣ���ֹ�����κδ���ӯ�����ʵĻ��" << endl;
	File.open(".\\land\\config.json");
	if (!File.is_open()) {
		writeConfigJson();
		cout << u8"���������ļ�: .\\land\\config.json" << endl;
	}
	File.close();
	File.open(".\\land\\land.json");
	if (!File.is_open()) {
		ofstream a(".\\land\\land.json", std::ios::out);
		a << "{}" ;
		a.close();
		cout << u8"���������ļ�: .\\land\\land.json" << endl;
	}
	File.close();
	File.open(".\\land\\player.json");
	if (!File.is_open()) {
		ofstream a(".\\land\\player.json", std::ios::out);
		a << "{}";
		a.close();
		cout << u8"���������ļ�: .\\land\\player.json" << endl;
	}
	File.close();

	LCONFIG = getjson(".\\land\\config.json");
	LANDCONFIG = getjson(".\\land\\land.json");
	PLAYERCONFIG = getjson(".\\land\\player.json");
}
//���ж��
void exit() {
	std::cout << u8"��ز��(land-g0.dll)�Ѿ�ж�ء�" << std::endl;
}

//===============д�������ļ�ģ��==================

//д���������ļ�
void writeConfigJson()
{
	//���ڵ�  
	Json::Value config;
	//���ڵ�����  
	config["money_buy"] = Json::Value(100);
	config["money_sell"] = Json::Value(100);
	config["max_land"] = Json::Value(5);
	config["itemid"] = Json::Value("345:0");
	config["max_size"] = Json::Value(10000);
	config["scoreboard"] = Json::Value("money");

	//ֱ�����
	//cout << "FastWriter:" << endl;
	//Json::FastWriter fw;
	//cout << fw.write(config) << endl << endl;

	//�������  
	//cout << "StyledWriter:" << endl;
	Json::StyledWriter sw1;
	//cout << sw1.write(config) << endl << endl;

	//������ļ�  
	ofstream os;
	os.open(".\\land\\config.json", std::ios::out);
	if (!os.is_open())
		cout << u8"[����]û���ҵ������ļ��� \" config.json\"." << endl;
	os << sw1.write(config);
	os.close();

	LCONFIG = sw1.write(config);
}
//д���������
void writePlayerJson(string uid,string land,string share_name,bool putblock, bool destroyblock, bool useitem, bool openchest, bool attack){
	string content = PLAYERCONFIG;
	Json::Value po;
	po["player"].append(uid);
	po["limit"];

	Json::Reader reader;
	Json::Value root;
	if (reader.parse(content, root)) {
		Json::Value po1;
		po1["putblock"] = Json::Value(putblock);
		po1["destroyblock"] = Json::Value(destroyblock);
		po1["useitem"] = Json::Value(useitem);
		po1["openchest"] = Json::Value(openchest);
		po1["attack"] = Json::Value(attack);
		po1["land"];
		po1["share_name"];

		const Json::Value arrayObj22 = root["limit"][uid]["land"];
		int length22 = arrayObj22.size();
		if (length22 > 0) {
			for (int i22 = 0; i22 < length22; i22++)
			{
				string b31 = root["limit"][uid]["land"][i22].asString();
				if (b31 != land) {
					po1["land"].append(b31);
				}
			}
		}
		if (land != "") {
			po1["land"].append(land);
		}
		const Json::Value arrayObj32 = root["limit"][uid]["share_name"];
		int length32 = arrayObj32.size();
		if (length32 > 0) {
			for (int i32 = 0; i32 < length32; i32++)
			{
				string b32 = root["limit"][uid]["share_name"][i32].asString();
				if (b32 != share_name) {
					po1["share_name"].append(b32);
				}
			}
		}
		if (share_name != "") {
			po1["share_name"].append(share_name);
		}

		po["limit"][uid] = Json::Value(po1);



		const Json::Value arrayObj = root["player"];
		int length = arrayObj.size();
		//cout<<length<<endl;
		if (length > 0) {
			for (int i = 0; i < length; i++)
			{
				string pot = root["player"][i].asString();

				if (pot != uid) {
					po["player"].append(pot);
					bool b5 = root["limit"][pot]["putblock"].asBool();
					bool b6 = root["limit"][pot]["destroyblock"].asBool();
					bool b7 = root["limit"][pot]["useitem"].asBool();
					bool b8 = root["limit"][pot]["openchest"].asBool();
					bool b9 = root["limit"][pot]["attack"].asBool();

					Json::Value po2;
					
					po2["putblock"] = Json::Value(b5);
					po2["destroyblock"] = Json::Value(b6);
					po2["useitem"] = Json::Value(b7);
					po2["openchest"] = Json::Value(b8);
					po2["attack"] = Json::Value(b9);
					po2["land"];
					po2["share_name"];
				
					const Json::Value arrayObj2 = root["limit"][pot]["land"];
					int length2 = arrayObj2.size();
					if (length2 > 0) {
						for (int i2 = 0; i2 < length2; i2++)
						{
							string b3 = root["limit"][pot]["land"][i2].asString();
							if (b3 != land) {
								po2["land"].append(b3);
							}
						}
					}

					const Json::Value arrayObj3 = root["limit"][pot]["share_name"];
					int length3 = arrayObj3.size();
					if (length3 > 0) {
						for (int i3 = 0; i3 < length3; i3++)
						{
							string b3 = root["limit"][pot]["share_name"][i3].asString();
							if (b3 != share_name) {
								po2["share_name"].append(b3);
							}
						}
					}
					po["limit"][pot] = Json::Value(po2);
				}
			}
		}
	}
	Json::StyledWriter sw;
	//cout << sw.write(po) << endl;

	//������ļ�  
	ofstream os;
	os.open(".\\land\\player.json", std::ios::out);
	if (!os.is_open())
		cout << u8"[����]û���ҵ������ļ�:  player.json" << endl;
	os << sw.write(po);
	os.close();

	PLAYERCONFIG = sw.write(po);
}
//�Ƴ����ӵ�е����
void removePlayerJson_land_sharename(string uid, string land,string sharename) {
	string content = PLAYERCONFIG;
	if (content == "" || content == "{}") { return; };
	Json::Value po;
	po["player"].append(uid);
	po["limit"];

	Json::Reader reader;
	Json::Value root;
	if (reader.parse(content, root)) {
		Json::Value po1;
		bool bb5 = root["limit"][uid]["putblock"].asBool();
		bool bb6 = root["limit"][uid]["destroyblock"].asBool();
		bool bb7 = root["limit"][uid]["useitem"].asBool();
		bool bb8 = root["limit"][uid]["openchest"].asBool();
		bool bb9 = root["limit"][uid]["attack"].asBool();

		po1["putblock"] = Json::Value(bb5);
		po1["destroyblock"] = Json::Value(bb6);
		po1["useitem"] = Json::Value(bb7);
		po1["openchest"] = Json::Value(bb8);
		po1["attack"] = Json::Value(bb9);
		po1["land"];
		po1["share_name"];

		const Json::Value arrayObj22 = root["limit"][uid]["land"];
		int length22 = arrayObj22.size();
		if (length22 > 0) {
			for (int i22 = 0; i22 < length22; i22++)
			{
				string b32 = root["limit"][uid]["land"][i22].asString();
				if (b32 != land) {
					po1["land"].append(b32);
				}
			}
		}

		const Json::Value arrayObj32 = root["limit"][uid]["share_name"];
		int length32 = arrayObj32.size();
		if (length32 > 0) {
			for (int i32 = 0; i32 < length32; i32++)
			{
				string b31 = root["limit"][uid]["share_name"][i32].asString();
				if (b31 != sharename) {
					po1["share_name"].append(b31);
				}
			}
		}

		po["limit"][uid] = Json::Value(po1);

		const Json::Value arrayObj = root["player"];
		int length = arrayObj.size();
		//cout<<length<<endl;
		if (length > 0) {
			for (int i = 0; i < length; i++)
			{
				string pot = root["player"][i].asString();

				if (pot != uid) {
					po["player"].append(pot);
					bool b5 = root["limit"][pot]["putblock"].asBool();
					bool b6 = root["limit"][pot]["destroyblock"].asBool();
					bool b7 = root["limit"][pot]["useitem"].asBool();
					bool b8 = root["limit"][pot]["openchest"].asBool();
					bool b9 = root["limit"][pot]["attack"].asBool();

					Json::Value po2;

					po2["putblock"] = Json::Value(b5);
					po2["destroyblock"] = Json::Value(b6);
					po2["useitem"] = Json::Value(b7);
					po2["openchest"] = Json::Value(b8);
					po2["attack"] = Json::Value(b9);
					po2["land"];
					po2["share_name"];

					const Json::Value arrayObj2 = root["limit"][pot]["land"];
					int length2 = arrayObj2.size();
					if (length2 > 0) {
						for (int i2 = 0; i2 < length2; i2++)
						{
							string b3 = root["limit"][pot]["land"][i2].asString();
							if (b3 != land) {
								po2["land"].append(b3);
							}
						}
					}

					const Json::Value arrayObj3 = root["limit"][pot]["share_name"];
					int length3 = arrayObj3.size();
					if (length3 > 0) {
						for (int i3 = 0; i3 < length3; i3++)
						{
							string b3 = root["limit"][pot]["share_name"][i3].asString();
							po2["share_name"].append(b3);
						}
					}
					po["limit"][pot] = Json::Value(po2);
				}
			}
		}
	}
	Json::StyledWriter sw;
	//cout << sw.write(po) << endl;

	//������ļ�  
	ofstream os;
	os.open(".\\land\\player.json", std::ios::out);
	if (!os.is_open())
		cout << u8"[����]û���ҵ������ļ�:  player.json" << endl;
	os << sw.write(po);
	os.close();

	PLAYERCONFIG = sw.write(po);
}
//��¼���
void writeLandJson(string uid, string point,string name)
{
	string content = LANDCONFIG;

	Json::Value po;
	po["po"].append(point);

	po["P_4"];

	po["P_2_xa"];
	po["P_2_xb"];
	po["P_2_za"];
	po["P_2_zb"];

	po["P_1_xa_za"];
	po["P_1_xb_za"];
	po["P_1_xa_zb"];
	po["P_1_xb_zb"];

	po["land"];
	//�ӽڵ�  
	Json::Value po1;
	po1["land_use"] = Json::Value(name);
	po1["xuid"] = Json::Value(uid);
	
	//�ӽڵ�ҵ����ڵ���  
	po["land"][point] = Json::Value(po1);

	auto aa = point.find(":");
	string aa2 = point.substr(0, aa);
	auto ab1 = aa2.find(".");
	auto ab2 = aa2.rfind(".");
	int x1 = std::stoi(aa2.substr(0, ab1));
	//int y1 = std::stoi(a2.substr(b1 + 1, b2 - b1 - 1));
	int z1 = std::stoi(aa2.substr(ab2 + 1));
	string aa3 = point.substr(aa + 1);
	auto ab3 = aa3.find(".");
	auto ab4 = aa3.rfind(".");
	int x2 = std::stoi(aa3.substr(0, ab3));
	//int y2 = std::stoi(a3.substr(b3 + 1, b4 - b3 - 1));
	int z2 = std::stoi(aa3.substr(ab4 + 1));
	whicthType ii = use_what_byxz(x1, z1, x2, z2);

	switch (ii)
	{
		//x�����꣬z������
	case P1111:
		po["P_1_xa_za"].append(point);//��һ����
		break;
	case P0101:
		po["P_1_xb_za"].append(point);//�ڶ�����
		break;
	case P0000:
		po["P_1_xb_zb"].append(point);//��������
		break;
	case P1010:
		po["P_1_xa_zb"].append(point);//��������
		break;

	case P1110:
		po["P_2_xa"].append(point);//��һ������
		break;
	case P1011:
		po["P_2_xa"].append(point);
		break;
	case P1101:
		po["P_2_za"].append(point);//һ������
		break;
	case P0111:
		po["P_2_zb"].append(point);
		break;
	case P1000:
		po["P_2_zb"].append(point);//��������
		break;
	case P0010:
		po["P_2_zb"].append(point);
		break;
	case P0100:
		po["P_2_xb"].append(point);//��������
		break;
	case P0001:
		po["P_2_xb"].append(point);
		break;

	case P0110:
		po["P_4"].append(point);//ȫ����
		break;
	case P1100:
		po["P_4"].append(point);
		break;
	case P0011:
		po["P_4"].append(point);
		break;
	case P1001:
		po["P_4"].append(point);
		break;
	default:
		cout << u8"[����]������󣬳��ִ˴�������ϵ����QQ: 654921949��" << endl;
		break;
	}
	//�����ֵ
	Json::Reader reader;
	Json::Value root;
	if (reader.parse(content, root)) {
		const Json::Value arrayObj = root["po"];
		int length = arrayObj.size();
		//cout<<length<<endl;
		if (length > 0) {
			for (int i = 0; i < length; i++)
			{
				string pot = root["po"][i].asString();
				if (pot != point) {
					string b1 = root["land"][pot]["land_use"].asString();
					string b4 = root["land"][pot]["xuid"].asString();

					po["po"].append(pot);

					Json::Value po2;
					po2["land_use"] = Json::Value(b1);
					po2["xuid"] = Json::Value(b4);

					po["land"][pot] = Json::Value(po2);

					auto aa = pot.find(":");
					string aa2 = pot.substr(0, aa);
					auto ab1 = aa2.find(".");
					auto ab2 = aa2.rfind(".");
					int x1 = std::stoi(aa2.substr(0, ab1));
					//int y1 = std::stoi(a2.substr(b1 + 1, b2 - b1 - 1));
					int z1 = std::stoi(aa2.substr(ab2 + 1));
					string aa3 = pot.substr(aa + 1);
					auto ab3 = aa3.find(".");
					auto ab4 = aa3.rfind(".");
					int x2 = std::stoi(aa3.substr(0, ab3));
					//int y2 = std::stoi(a3.substr(b3 + 1, b4 - b3 - 1));
					int z2 = std::stoi(aa3.substr(ab4 + 1));
					whicthType ii = use_what_byxz(x1, z1, x2, z2);
					
					switch (ii)
					{
						//x�����꣬z������
					case P1111:
						po["P_1_xa_za"].append(pot);//��һ����
						break;
					case P0101:
						po["P_1_xb_za"].append(pot);//�ڶ�����
						break;
					case P0000:
						po["P_1_xb_zb"].append(pot);//��������
						break;
					case P1010:
						po["P_1_xa_zb"].append(pot);//��������
						break;

					case P1110:
						po["P_2_xa"].append(pot);//��һ������
						break;
					case P1011:
						po["P_2_xa"].append(pot);
						break;
					case P1101:
						po["P_2_za"].append(pot);//һ������
						break;
					case P0111:
						po["P_2_zb"].append(pot);
						break;
					case P1000:
						po["P_2_zb"].append(pot);//��������
						break;
					case P0010:
						po["P_2_zb"].append(pot);
						break;
					case P0100:
						po["P_2_xb"].append(pot);//��������
						break;
					case P0001:
						po["P_2_xb"].append(pot);
						break;

					case P0110:
						po["P_4"].append(pot);//ȫ����
						break;
					case P1100:
						po["P_4"].append(pot);
						break;
					case P0011:
						po["P_4"].append(pot);
						break;
					case P1001:
						po["P_4"].append(pot);
						break;
					default:
						cout << u8"[����]������󣬳��ִ˴�������ϵ����QQ: 654921949��" << endl;
						break;
					}
				}
			}
		}
	}
	else {
		//return;
	}

	//ֱ�����  
	//cout << "FastWriter:" << endl;
	//Json::FastWriter fw;
	//cout << fw.write(root) << endl << endl;


	//�������  
	//cout << "StyledWriter:" << endl;
	Json::StyledWriter sw;
	//cout << sw.write(po) << endl;

	//������ļ�  
	ofstream os;
	os.open(".\\land\\land.json", std::ios::out);
	if (!os.is_open())
		cout << u8"[����]û���ҵ������ļ�:  land.json" << endl;
	os << sw.write(po);
	os.close();

	LANDCONFIG = sw.write(po);
}
//�Ƴ����
void removeLandJson(string point) {
	if (LANDCONFIG == "" || LANDCONFIG == "{}") { return; }
	string content = LANDCONFIG;
	Json::Value po;

	Json::Reader reader;
	Json::Value root;
	if (reader.parse(content, root)) {
		const Json::Value arrayObj = root["po"];
		int length = arrayObj.size();
		//cout<<length<<endl;
		auto ab1 = point.find(".");
		auto ab2 = point.rfind(".");
		int x = std::stoi(point.substr(0, ab1));
		int y = std::stoi(point.substr(ab1 + 1, ab2 - ab1 - 1));
		int z = std::stoi(point.substr(ab2 + 1));
		string land_who = island(x, y, z);

		if (length > 0) {
			for (int i = 0; i < length; i++)
			{
				string pot = root["po"][i].asString();
				if (pot != land_who) {
					
					po["po"].append(pot);

					po["P_4"];

					po["P_2_xa"];
					po["P_2_xb"];
					po["P_2_za"];
					po["P_2_zb"];

					po["P_1_xa_za"];
					po["P_1_xb_za"];
					po["P_1_xa_zb"];
					po["P_1_xb_zb"];

					po["land"];

					string b1 = root["land"][pot]["land_use"].asString();
					string b4 = root["land"][pot]["xuid"].asString();


					Json::Value po2;
					po2["land_use"] = Json::Value(b1);
					po2["xuid"] = Json::Value(b4);

					po["land"][pot] = Json::Value(po2);

					auto aa = pot.find(":");
					string aa2 = pot.substr(0, aa);
					auto ab1 = aa2.find(".");
					auto ab2 = aa2.rfind(".");
					int x1 = std::stoi(aa2.substr(0, ab1));
					//int y1 = std::stoi(a2.substr(b1 + 1, b2 - b1 - 1));
					int z1 = std::stoi(aa2.substr(ab2 + 1));
					string aa3 = pot.substr(aa + 1);
					auto ab3 = aa3.find(".");
					auto ab4 = aa3.rfind(".");
					int x2 = std::stoi(aa3.substr(0, ab3));
					//int y2 = std::stoi(a3.substr(b3 + 1, b4 - b3 - 1));
					int z2 = std::stoi(aa3.substr(ab4 + 1));
					whicthType ii = use_what_byxz(x1, z1, x2, z2);

					switch (ii)
					{
						//x�����꣬z������
					case P1111:
						po["P_1_xa_za"].append(pot);//��һ����
						break;
					case P0101:
						po["P_1_xb_za"].append(pot);//�ڶ�����
						break;
					case P0000:
						po["P_1_xb_zb"].append(pot);//��������
						break;
					case P1010:
						po["P_1_xa_zb"].append(pot);//��������
						break;

					case P1110:
						po["P_2_xa"].append(pot);//��һ������
						break;
					case P1011:
						po["P_2_xa"].append(pot);
						break;
					case P1101:
						po["P_2_za"].append(pot);//һ������
						break;
					case P0111:
						po["P_2_zb"].append(pot);
						break;
					case P1000:
						po["P_2_zb"].append(pot);//��������
						break;
					case P0010:
						po["P_2_zb"].append(pot);
						break;
					case P0100:
						po["P_2_xb"].append(pot);//��������
						break;
					case P0001:
						po["P_2_xb"].append(pot);
						break;

					case P0110:
						po["P_4"].append(pot);//ȫ����
						break;
					case P1100:
						po["P_4"].append(pot);
						break;
					case P0011:
						po["P_4"].append(pot);
						break;
					case P1001:
						po["P_4"].append(pot);
						break;
					default:
						cout << u8"[����]������󣬳��ִ˴�������ϵ����QQ: 654921949��" << endl;
						break;
					}
				}
			}
		}
	}
	else {
		//return;
	}

	Json::StyledWriter sw;

	ofstream os;
	os.open(".\\land\\land.json", std::ios::out);
	if (!os.is_open())
		cout << u8"[����]û���ҵ������ļ�:  land.json" << endl;
	os << sw.write(po);
	os.close();

	LANDCONFIG = sw.write(po);
};

//==================��ȡ�����ļ���Ϣ=================================

string getjson(string name) {
	ifstream in;
	in.open(name);
	std::string content((std::istreambuf_iterator<char>(in)), (std::istreambuf_iterator<char>()));
	cout << u8"[��ز��]��ȡ�����ļ�: "<<name<< endl;
	in.close();
	if (content == "") { return "{}"; }
	Json::Reader reader;
	Json::Value root;
	if (reader.parse(content, root) || content == "{}") {

		return content;
	}
	else {
		cout << u8"[����]�޷���ȷ��ȡ�����ļ�: " << name << endl;
		cout << u8"\n��ز�����޷�����ʹ�ã�" << endl;
		return "";
	}
	return content;
}
int get_maxsize() {
	string content = LCONFIG;

	Json::Reader reader;
	Json::Value root;
	if (reader.parse(content, root)) {
		int maxsize = root["max_size"].asInt();
		return maxsize;
	}
	else {
		cout << u8"[����]�޷���ȷ��ȡ�����ļ�: .\\land\\config.json\n" << endl;
	}
	return 0;
}
int get_maxland() {
	string content = LCONFIG;

	Json::Reader reader;
	Json::Value root;
	if (reader.parse(content, root)) {
		int maxland = root["max_land"].asInt();
		return maxland;
	}
	else {
		cout << u8"[����]�޷���ȷ��ȡ�����ļ�: .\\land\\config.json\n" << endl;
	}
	return 0;
}
string get_setmoney() {
	string content = LCONFIG;

	Json::Reader reader;
	Json::Value root;
	if (reader.parse(content, root)) {
		string scoreboard = root["scoreboard"].asString();
		return scoreboard;
	}
	else {
		cout << u8"[����]�޷���ȷ��ȡ�����ļ�: .\\land\\config.json\n" << endl;
	}
	return "money";
}
int get_landmoney(bool a){
	string content = LCONFIG;

	Json::Reader reader;
	Json::Value root;
	if (reader.parse(content, root)) {
		int landmoney;
		if (a) {
			landmoney = root["money_buy"].asInt();
		}
		else {
			landmoney = root["money_sell"].asInt();
		}
		return landmoney;
	}
	else {
		cout << u8"[����]�޷���ȷ��ȡ�����ļ�: .\\land\\config.json\n" << endl;
	}
	return 0;
}
//��ȡ��ص������Ϣ
string get_land_json(string point, string name) {
	string content = LANDCONFIG;

	Json::Reader reader;
	Json::Value root;
	if (reader.parse(content, root)) {
		return root["land"][point][name].asString();
	}
	else {
		//cout << u8"[����]�޷���ȷ��ȡ�����ļ�: land.json\n" << endl;
	}
	return "0";
};
int get_player_land_num(string uid) {
	string content = PLAYERCONFIG;

	Json::Reader reader;
	Json::Value root;
	if (reader.parse(content, root)) {
		Json::Value arrayObj22 = root["limit"][uid]["land"];
		int maxland = arrayObj22.size();
		return maxland;
	}
	return 0;
}
void get_player_json(string uid, bool* putblock, bool* destroyblock, bool* useitem, bool* openchest, bool* attack) {
	string content = PLAYERCONFIG;

	Json::Reader reader;
	Json::Value root;
	if (reader.parse(content, root)) {
		*putblock = root["limit"][uid]["putblock"].asBool();
		*destroyblock = root["limit"][uid]["destroyblock"].asBool();
		*useitem = root["limit"][uid]["useitem"].asBool();
		*openchest = root["limit"][uid]["openchest"].asBool();
		*attack = root["limit"][uid]["attack"].asBool();
	}
	else {
		*putblock = false;
		*destroyblock = false;
		*useitem = false;
		*openchest = false;
		*attack = false;
	}
	return;
};

//==============��أ���ģ�飩============================================

//�жϵ���������Ʒid
bool isitemid(int id, int id2) {
	string content = LCONFIG;
	Json::Reader reader;
	Json::Value root;
	if (reader.parse(content, root)) {
		string jsonid = root["itemid"].asString();
		char itemid[256];
		
		sprintf_s(itemid, "%d:%d", id, id2);
		if (itemid == jsonid) {
			return true;
		}
	}
	else {
		cout << u8"[����]�޷���ȷ��ȡ�����ļ�: config.json\n" << endl;
	}
	return false;
}
//λ���жϣ����Ż�������ѭ��������
string island(int x, int y, int z) {
	if (LANDCONFIG == "" || LANDCONFIG == "{}") { return "0"; }
	string po;
	string check_p4 = island_circle("P_4", x, y, z);
	if (check_p4 == "0") {
		if (x >= 0) {
			string chaeck_p2_xa = island_circle("P_2_xa", x, y, z);
			if (chaeck_p2_xa == "0") {
				if (z >= 0) {
					string chaeck_p2_za = island_circle("P_2_za", x, y, z);
					if (chaeck_p2_za == "0") {
						string chaeck_p1_xa_za = island_circle("P_1_xa_za", x, y, z);
						return chaeck_p1_xa_za;
					}
					return chaeck_p2_za;
				}
				else {
					string chaeck_p2_zb = island_circle("P_2_zb", x, y, z);
					if (chaeck_p2_zb == "0") {
						string chaeck_p1_xa_zb = island_circle("P_1_xa_zb", x, y, z);
						return chaeck_p1_xa_zb;
					}
					return chaeck_p2_zb;
				}
			}
			return chaeck_p2_xa;

		}
		else {
			string chaeck_p2_xb = island_circle("P_2_xb", x, y, z);
			if (chaeck_p2_xb == "0") {
				if (z >= 0) {
					string chaeck_p2_za = island_circle("P_2_za", x, y, z);
					if (chaeck_p2_za == "0") {
						string chaeck_p1_xb_za = island_circle("P_1_xb_za", x, y, z);
						return chaeck_p1_xb_za;
					}
					return chaeck_p2_za;
				}
				else {
					string chaeck_p2_zb = island_circle("P_2_zb", x, y, z);
					if (chaeck_p2_zb == "0") {
						string chaeck_p1_xb_zb = island_circle("P_1_xb_zb", x, y, z);
						return chaeck_p1_xb_zb;
					}
					return chaeck_p2_zb;
				}
			}
			return chaeck_p2_xb;
		}
	}
	else {
		return check_p4;
	}
	return "0";
}
//ִ���Ż�
whicthType use_what_byxz(int x1, int z1, int x2, int z2) {
	if (x1 >= 0 && z1 >= 0 && x2 >= 0 && z2 >= 0) {
		return P1111;
	}
	if (x1 >= 0 && z1 >= 0 && x2 >= 0 && z2 < 0) {
		return P1110;
	}
	if (x1 >= 0 && z1 >= 0 && x2 < 0 && z2 >= 0) {
		return P1101;
	}
	if (x1 >= 0 && z1 >= 0 && x2 < 0 && z2 < 0) {
		return P1100;
	}
	if (x1 >= 0 && z1 < 0 && x2 >= 0 && z2 >= 0) {
		return P1011;
	}
	if (x1 >= 0 && z1 < 0 && x2 >= 0 && z2 < 0) {
		return P1010;
	}
	if (x1 >= 0 && z1 < 0 && x2 < 0 && z2 >= 0) {
		return P1001;
	}
	if (x1 >= 0 && z1 < 0 && x2 < 0 && z2 < 0) {
		return P1000;
	}

	if (x1 < 0 && z1 >= 0 && x2 >= 0 && z2 >= 0) {
		return P0111;
	}
	if (x1 < 0 && z1 >= 0 && x2 >= 0 && z2 < 0) {
		return P0110;
	}
	if (x1 < 0 && z1 >= 0 && x2 < 0 && z2 >= 0) {
		return P0101;
	}
	if (x1 < 0 && z1 >= 0 && x2 < 0 && z2 < 0) {
		return P0100;
	}
	if (x1 < 0 && z1 < 0 && x2 >= 0 && z2 >= 0) {
		return P0011;
	}
	if (x1 < 0 && z1 < 0 && x2 >= 0 && z2 < 0) {
		return P0010;
	}
	if (x1 < 0 && z1 < 0 && x2 < 0 && z2 >= 0) {
		return P0001;
	}
	if (x1 < 0 && z1 < 0 && x2 < 0 && z2 < 0) {
		return P0000;
	}
	return PBUG;
}
string island_circle(string point,int x,int y,int z) {
	string content = LANDCONFIG;
	Json::Reader reader;
	Json::Value root;
	if (reader.parse(content, root)) {
		const Json::Value arrayObj = root[point];
		//cout << "ll" << arrayObj << endl;
		int length = arrayObj.size();
		//cout<<length<<endl;
		if (length > 0) {
			for (int i = 0; i < length; i++)
			{
				string po = root[point][i].asString();

				auto a = po.find(":");
				string a2 = po.substr(0, a);
				auto b1 = a2.find(".");
				auto b2 = a2.rfind(".");
				int x1 = std::stoi(a2.substr(0, b1));
				int y1 = std::stoi(a2.substr(b1 + 1, b2 - b1 - 1));
				int z1 = std::stoi(a2.substr(b2 + 1));
				string a3 = po.substr(a + 1);
				auto b3 = a3.find(".");
				auto b4 = a3.rfind(".");
				int x2 = std::stoi(a3.substr(0, b3));
				int y2 = std::stoi(a3.substr(b3 + 1, b4 - b3 - 1));
				int z2 = std::stoi(a3.substr(b4 + 1));

				//�����ΧһȦ�޷�ʹ����Ʒ
				if (debug_useitem_open[x+y+z]) {
					if( x1 >= 0 ){
						x1 = x1+ 1;
					}
					else {
						x1 = x1-1;
					}
					if (x2 >= 0) {
						x2 = x2+1;
					}
					else {
						x2 = x2-1;
					}
					if (z1 >= 0) {
						z1 = z1+1;
					}
					else {
						z1 = z1-1;
					}
					if (z2 >= 0) {
						z2 =z2+ 1;
					}
					else {
						z2 =z2-1;
					}
				}
				bool is = land_is(x, y, z, x1, y1, z1, x2, y2, z2);
				//cout << po<< endl;
				if (is) {
					return po;
				}
			}
		}
	}else {
		return "0";
	}
	return "0";
}
//Ȧ���ж�
string checkland(int xa, int ya, int za, int xb, int yb, int zb) {
	string content = LANDCONFIG;
	Json::Reader reader;
	Json::Value root;
	if (reader.parse(content, root)) {
		const Json::Value arrayObj = root["po"];
		int length = arrayObj.size();
		//cout<<length<<endl;
		if (length > 0) {
			for (int i = 0; i < length; i++)
			{
				string po = root["po"][i].asString();

				auto a = po.find(":");
				string a2 = po.substr(0, a);
				auto b1 = a2.find(".");
				auto b2 = a2.rfind(".");
				int x1 = std::stoi(a2.substr(0, b1));
				int y1 = std::stoi(a2.substr(b1 + 1, b2 - b1 - 1));
				int z1 = std::stoi(a2.substr(b2 + 1));
				string a3 = po.substr(a + 1);
				auto b3 = a3.find(".");
				auto b4 = a3.rfind(".");
				int x2 = std::stoi(a3.substr(0, b3));
				int y2 = std::stoi(a3.substr(b3 + 1, b4 - b3 - 1));
				int z2 = std::stoi(a3.substr(b4 + 1));
				//cout << "x1:" << x1 << ",y1:" << y1 << ",z1:" << z1 << ",x2:" << x2 << ",y2:" << y2 << ",z2:" << z2 << endl;
				if (land_is(xa, ya, za, x1, y1, z1, x2, y2, z2)) { return po; }
				if (land_is(xb, yb, zb, x1, y1, z1, x2, y2, z2)) { return po; }
				if (land_is(xb, ya, za, x1, y1, z1, x2, y2, z2)) { return po; }
				if (land_is(xa, yb, zb, x1, y1, z1, x2, y2, z2)) { return po; }
				if (land_is(x1, y1, z1, xa, ya, za, xb, yb, zb)) { return po; }
				if (land_is(x2, y2, z2, xa, ya, za, xb, yb, zb)) { return po; }
				if (land_is(x2, y1, z1, xa, ya, za, xb, yb, zb)) { return po; }
				if (land_is(x1, y2, z2, xa, ya, za, xb, yb, zb)) { return po; }
			}
		}
	}
	else {
		return "0";
	}
	return "0";
}
//�ж�һ�����Ƿ���һ��������
bool land_is(int x, int y, int z, int x1, int y1, int z1, int x2, int y2, int z2) {
	if (x1 < x2) {
		if (x >= x1 && x <= x2) {
			if (z1 < z2) {
				if (z >= z1 && z <= z2) {
					return true;
				}
			}
			else {
				if (z >= z2 && z <= z1) {
					return true;
				}
			}
		}
	}
	else {
		if (x <= x1 && x >= x2) {
			if (z1 < z2) {
				if (z >= z1 && z <= z2) {
					return true;
				}
			}
			else {
				if (z >= z2 && z <= z1) {
					return true;
				}
			}
		}
	}
	return false;
}
//�Ƿ�ӵ����ص�Ȩ��
bool island_player(string xuid,string name,string point/*x.y.z*/, landType id) {
	auto b1 = point.find(".");
	auto b2 = point.rfind(".");
	int x = std::stoi(point.substr(0, b1));
	int y = std::stoi(point.substr(b1 + 1, b2 - b1 - 1));
	int z = std::stoi(point.substr(b2 + 1));
	if (id == USEITEM) {
		debug_useitem_open[x+y+z] = true;
	}
	//cout << "ll" << endl;
	//�Ƿ������
	string landis = island(x, y, z);
	if (debug_useitem_open[x+y+z]) {
		debug_useitem_open[x+y+z] = false;
		debug_useitem_open.erase(x + y + z);
	}
	if (landis == "0") {
		return true;
	}
	//�Ƿ����������
	string landop = get_land_json(landis,"xuid");
	if (landop == xuid){
		return true;
	}

	string content = PLAYERCONFIG;
	Json::Reader reader;
	Json::Value root;

	if (reader.parse(content, root)) {
		int i = 0;
		const Json::Value arrayObj3 = root["limit"][landop]["share_name"];
		int length3 = arrayObj3.size();
		if (length3 > 0) {
			for (int i3 = 0; i3 < length3; i3++)
			{
				string b4 = root["limit"][landop]["share_name"][i3].asString();
				if (b4 ==name) {
					i++;
					break;
				}
			}
		}
		if (i > 0 ) {
			if (id == PUTBLOCK) {
				if (root["limit"][landop]["putblock"].asBool()) { return true; }
				return false;
			}
			if (id == DESTROYBLOCK) {
				if (root["limit"][landop]["destroyblock"].asBool()) { return true; }
				return false;
			}
			if (id == USEITEM) {
				if (root["limit"][landop]["useitem"].asBool()) { return true; }
				return false;
			}
			if (id == OPENCHEST) {
				if (root["limit"][landop]["openchest"].asBool()) { return true; }
				return false;
			}
			if (id == ATTACKT) {
				if (root["limit"][landop]["attack"].asBool()) { return true; }
				return false;
			}
		}
	}
	return false;
}
//��ع���
bool land_buy(string uid, int* s, int* money) {
	string poa = playerSetA[uid];
	string pob = playerSetB[uid];
	if (poa == "0" || pob == "0") {
		return false;
	}
	auto b1 = poa.find(".");
	auto b2 = poa.rfind(".");
	int x1 = std::stoi(poa.substr(0, b1));
	int y1 = std::stoi(poa.substr(b1 + 1, b2 - b1 - 1));
	int z1 = std::stoi(poa.substr(b2 + 1));

	auto b3 = pob.find(".");
	auto b4 = pob.rfind(".");
	int x2 = std::stoi(pob.substr(0, b3));
	int y2 = std::stoi(pob.substr(b3 + 1, b4 - b3 - 1));
	int z2 = std::stoi(pob.substr(b4 + 1));

	int dx = abs(abs(x1) - abs(x2))+ 1;
	int dz = abs(abs(z1) - abs(z2))+ 1;

	*s = dx * dz;
	*money = (*s) * (get_landmoney(true));

	int a = *s;
	int b = *money;
	string land_who = checkland(x1, y1, z1, x2, y2, z2);
	if (land_who != "0") {
		string str = u8"��c����ص����޷��������ء�\n��e�ص��������: ";
		str.append(land_who);
		sendText(uid, str, RAW);
		return false;
	}
	
	int maxsize= get_maxsize();
	//cout << maxsize << endl;
	if (a > maxsize) {
		char str[256];
		sprintf_s(str, u8"��c����ʧ�ܣ���سߴ糬���������: %d",maxsize);
		sendText(uid, str, RAW);
		return false;
	}
	int maxlandnum = get_maxland();
	if (get_player_land_num(uid) >= maxlandnum) {
		char str[256];
		sprintf_s(str, u8"��c����ʧ�ܣ�������������������: %d",maxlandnum);
		sendText(uid, str, RAW);
		return false;
	}
	if (b <= getmoney(uid)) {
		return true;
	}
	else {
		sendText(uid, u8"��c���㣬����ʧ�ܡ�", RAW);
		return false;
	}

	return false;
}
//��س���
bool land_sell(string uid,string point,int* money,string* landpot){
	auto b1 = point.find(".");
	auto b2 = point.rfind(".");
	int x = std::stoi(point.substr(0, b1));
	int y = std::stoi(point.substr(b1 + 1, b2 - b1 - 1));
	int z = std::stoi(point.substr(b2 + 1));

	string landpo = island(x,y,z);
	*landpot = landpo;
	if (landpo == "0") {
		sendText(uid,u8"��c��Ľ���û����أ�",RAW);
		return false;
	}
	string land_who = get_land_json(landpo, "xuid");
	if (uid != land_who) {
		sendText(uid, u8"��c�����ز������㣡", RAW);
		return false;
	}
	else {
		auto aa = landpo.find(":");
		string poa = landpo.substr(0, aa);
		string pob = landpo.substr(aa + 1);

		auto b1 = poa.find(".");
		auto b2 = poa.rfind(".");
		int x1 = std::stoi(poa.substr(0, b1));
		int y1 = std::stoi(poa.substr(b1 + 1, b2 - b1 - 1));
		int z1 = std::stoi(poa.substr(b2 + 1));

		auto b3 = pob.find(".");
		auto b4 = pob.rfind(".");
		int x2 = std::stoi(pob.substr(0, b3));
		int y2 = std::stoi(pob.substr(b3 + 1, b4 - b3 - 1));
		int z2 = std::stoi(pob.substr(b4 + 1));

		int dx = abs(abs(x1) - abs(x2)) + 1;
		int dz = abs(abs(z1) - abs(z2)) + 1;
		*money = dx * dz * (get_landmoney(false));

		return true;
	}
	return false;
}

//=========MC�ű�=========

static VA p_spscqueue;
// ִ�к��ָ��
static bool runcmd(std::string cmd) {
	if (p_spscqueue != 0)
		return SYMCALL(bool, MSSYM_MD5_b5c9e566146b3136e6fb37f0c080d91e, p_spscqueue, cmd);
	return false;
}
//����ҷ����ı���Ϣ
void sendText(string xuid, string text, TextType tp) {
	Player* p = onlinePlayers[xuid];
	string name = p->getNameTag();
	runcmd("tellraw " + name + " {\"rawtext\":[{\"text\":\"" + text + "\"}]}");

}

//=============�Ʒְ徭��ģ��=============
Scoreboard* landmoney_scoreboard;
int getmoney(string xuid) {
	Player* p = onlinePlayers[xuid];
	string objname = get_setmoney();
	auto testobj = landmoney_scoreboard->getobject(&objname);
	if (!testobj) {
		cout << u8"û���ҵ���Ӧ�Ʒְ壬�Զ�����: "<<objname << endl;
		runcmd("scoreboard objectives add \"" + objname + "\" dummy money");
		return 0;
	}
	__int64 a2[2];
	auto scoreid = landmoney_scoreboard->getScoreboardID(p);
	auto scores = testobj->getplayerscoreinfo((ScoreInfo*)a2, scoreid);
	return (__int64)scores->getcount();
}
void addmoney(string xuid,int money) {
	Player* p = onlinePlayers[xuid];
	string playername = p->getNameTag();

	string objname = get_setmoney();

	auto testobj = landmoney_scoreboard->getobject(&objname);

	if (!testobj) {
		cout << u8"û���ҵ���Ӧ�Ʒְ壬�Զ�����: " << objname << endl;
		runcmd("scoreboard objectives add \"" + objname + "\" dummy money");
	}

	runcmd("scoreboard players add \"" + playername + "\" \"" + objname + "\" " + std::to_string(money));
}
bool removemoney(string xuid, int money) {
	Player* p = onlinePlayers[xuid];
	string playername = p->getNameTag();

	int playermoney = getmoney(xuid);
	string objname = get_setmoney();

	if (money < playermoney) {
		runcmd("scoreboard players remove \"" + playername + "\" \"" + objname + "\" " + std::to_string(money));
		return true;
	}

	return false;
}

//===============guiģ��==================
//��ȡ��ͼ��Ϣ(���ڻ�ȡxuid)
static VA pxuid_level;
THook(Player*, MSSYM_MD5_c4b0cddb50ed88e87acce18b5bd3fb8a,
	Player* _this, VA level, __int64 a3, int a4, __int64 a5, __int64 a6, void* xuid, std::string& strxuid, __int64* a9, __int64 a10, __int64 a11) {
	pxuid_level = level;
	return original(_this, level, a3, a4, a5, a6, xuid, strxuid, a9, a10, a11);
}

// ���ѡ���
THook(void, MSSYM_MD5_8b7f7560f9f8353e6e9b16449ca999d2,
	VA _this, VA id, VA handle, ModalFormResponsePacket** fp) {
	
	ModalFormResponsePacket* fmp = *fp;

	Player* p = SYMCALL(Player*, MSSYM_B2QUE15getServerPlayerB1AE20ServerNetworkHandlerB2AAE20AEAAPEAVServerPlayerB2AAE21AEBVNetworkIdentifierB2AAA1EB1AA1Z,
		handle, id, *(char*)((VA)fmp + 16));
	string point = p->getPos()->toJsonString();
	string  uid = p->getXuid(pxuid_level);

	if (p != NULL) {
		UINT fid = fmp->getFormId();
		if (destroyForm(fid)) {
			auto formid = fid;
			auto selected = fmp->getSelectStr();
			if (formid == gui_helpfid[uid]) {
				if (selected == "0") {//�������ѡ��
					playerOpen[uid] = true;
					TextType tp{ RAW };
					sendText(uid, u8"��a�ѿ������ѡ��ģʽ\n��d���÷����ѡ���A\n��e���ֵ��������˳����ѡ��ģʽ", tp);
				}
				if (selected == "1") {//�ر����ѡ��
					playerOpen.erase(uid);
					playerSetA[uid]="0";
					playerSetB[uid]="0";

					string say = u8"��c�˳����ѡ��ģʽ";

					sendText(uid, say, RAW);
				}
				if (selected == "2") {//��س���
					int money;
					string landpo;
					if (land_sell(uid, point, &money,&landpo)) {
						string str2 = u8"��e��ȷ�����Ƿ�Ҫ���۽������: ";
						str2.append(landpo);
						str2.append(u8"\n��b����ý��: ");
						char aa[128];
						sprintf_s(aa, u8"%d", money);
						str2.append(aa);

						gui_sellfid[uid] = send_landsellgui(uid, u8"��b�Ƿ���۸����", str2, u8"ȷ��", u8"ȡ��");
					}
					else {
						sendText(uid, u8"��c�޷��������", RAW);
					}
				}
				if (selected == "3") {//�����Ϣ
					land_info(uid, point);
				}
				if (selected == "4") {//�������
					gui_landsharefid[uid] = send_landshare_gui(uid);
				}
				if (selected == "5") {//Ȩ������
					gui_landpermissionfid[uid]=send_landpermission_gui(uid);
				}
				gui_helpfid.erase(uid);
			};
			//��ع���
			if (formid == gui_buyfid[uid]) {
				if (selected == "true") {
					int s;
					int money;
					if (land_buy(uid, &s, &money)) {
						if (removemoney(uid, money)) {
							string playername = p->getNameTag();

							string point = playerSetA[uid];
							point.append(":");
							point.append(playerSetB[uid]);

							char str[256];
							writeLandJson(uid, point, playername);
							writePlayerJson(uid, point, "", false, false, false, false, false);

							sprintf_s(str, u8"��a�ɹ��������,����: %d", money);
							sendText(uid, str, RAW);
						}
						else {
							sendText(uid, u8"��c����ʧ�ܡ�", RAW);
						}
						playerOpen.erase(uid);
						playerSetA[uid]="0";
						playerSetB[uid]="0";
					}
					else {
						sendText(uid, u8"��c����ʧ�ܣ�������Ȧ�ء�", RAW);
					}
				}
				if (selected == "false") {
					sendText(uid, u8"��c������Ȧ��\n��e���ֵ��������˳����ѡ��ģʽ", RAW);
				}
				gui_buyfid.erase(uid);
			};
			//��س���
			if (formid == gui_sellfid[uid]) {
				if (selected == "true") {
					int money;
					string landpo;
					string point = p->getPos()->toJsonString();
					if (land_sell(uid, point, &money,&landpo)) {
						removeLandJson(landpo);
						removePlayerJson_land_sharename(uid, landpo, "");
						string str;
						char str2[256];
						addmoney(uid, money);
						sprintf_s(str2, u8"��a�ɹ�������أ���ý��: %d", money);
						str.append(str2);
						sendText(uid, str, RAW);
					}
					else {
						sendText(uid, u8"��c����ʧ�ܣ�", RAW);
					}

				}
				if (selected == "false") {
					sendText(uid, u8"��dȡ����س���", RAW);
				}
				
				gui_sellfid.erase(uid);
			};
			if (formid == gui_landsharefid[uid]) {
				//cout << gui_land_sharename[uid] << endl;
				string id1 = selected.substr(1, 1);
				//cout << id1 << "...." << id2 << endl;
				if (id1 != "0" && id1 != "1") {
					//sendText(uid, u8"����һ����ֵĴ��������·���", RAW);
				}
				else {
					int a = selected.find(",");
					int b = selected.size();
					string id2 = selected.substr(a+1);
					if (id1 == "0") {
						Json::Reader reader;
						Json::Value root;
						string namelist = gui_land_sharename[uid];
						if (reader.parse(namelist, root)) {
							string sharename = root["name"][id2].asString();
							bool putblock; bool destroyblock; bool useitem; bool openchest; bool attack;

							get_player_json(uid, &putblock, &destroyblock, &useitem, &openchest, &attack);
							writePlayerJson(uid, "",sharename, putblock, destroyblock, useitem, openchest, attack);
							sendText(uid, u8"��d����ɹ���", RAW);
						}
					}
					if (id1 == "1") {
						Json::Reader reader;
						Json::Value root;
						string namelist = gui_land_sharename[uid];
						if (reader.parse(namelist, root)) {
							string sharename = root["name"][id2].asString();

							removePlayerJson_land_sharename(uid, "", sharename);
							sendText(uid, u8"��dȡ������ɹ���", RAW);
						}
					}

					gui_land_sharename.erase(uid);
					gui_landsharefid.erase(uid);
				};
			}
			if (formid == gui_landpermissionfid[uid]) {
				int len = selected.size();

				int a1 = selected.find(",");
				string b1 = selected.substr(1, a1 - 1);
				string c1 = selected.substr(a1 + 1);

				int a2 = c1.find(",");
				string b2 = selected.substr(a1 + 1, a2 );
				string c2 = c1.substr(a2 + 1);

				int a3 = c2.find(",");
				string b3 = selected.substr(a1 + a2 + 2, a3 );
				string c3 = c2.substr(a3 + 1);

				int a4 = c3.find(",");
				string b4 = selected.substr(a1+a2+a3+3, a4);
				string c4 = c3.substr(a4 + 1);

				string b5 = selected.substr(a1+a2+a3+a4+4);
				if (b5 == "true]" || b5 == "false]") {
					bool putblock; bool destroyblock; bool useitem; bool openchest; bool attack;
					if (b1 == "true") {
						putblock = true;
					}
					else {
						putblock = false;
					}
					if (b2 == "true") {
						destroyblock = true;
					}
					else {
						destroyblock = false;
					}
					if (b3 == "true") {
						useitem = true;
					}
					else {
						useitem = false;
					}
					if (b4 == "true") {
						openchest = true;
					}
					else {
						openchest = false;
					}
					if (b5 == "true]") {
						attack = true;
					}
					else {
						attack = false;
					}
					writePlayerJson(uid,"","", putblock, destroyblock, useitem, openchest, attack);
					sendText(uid, u8"��a���óɹ�", RAW);
				}
				gui_landpermissionfid.erase(uid);
			}
		}
	}
	
	original(_this, id, handle, fp);
}
static unsigned getFormId() {
	unsigned id = time(0);
	do {
		--id;
	} while (id == 0 || fids[id]);
	fids[id] = true;
	return id;
}
bool destroyForm(unsigned fid)
{
	if (fids[fid]) {
		fids.erase(fid);
		return true;
	}
	return false;
}
unsigned sendForm(string uuid, std::string str)
{
	Player* pPlayer = onlinePlayers[uuid];
	if (pPlayer == 0)
		return 0;
	auto payload = str;
	WBStream ws;
	unsigned fid = getFormId();
	ws.apply(VarUInt(fid));
	ws.apply(VarUInt(payload.size()));
	ws.write(payload.c_str(), payload.size());
	MyPkt<100, false> guipk{ ws.data };
	SYMCALL(VA, MSSYM_B1QE17sendNetworkPacketB1AE12ServerPlayerB2AAE15UEBAXAEAVPacketB3AAAA1Z,
		pPlayer, &guipk);

	return fid;

}

// �ܲ˵�
static UINT landhelp_gui(string xuid) {
	Json::Value jv;
	jv["type"] = "form";
	jv["title"] = u8"���ϵͳ";
	jv["content"] = u8"��eѡ��";
	jv["buttons"];

	Json::Value jv2;
	jv2["text"] = u8"��9�������ѡ��ģʽ";
	Json::Value jv3;
	jv3["text"] = u8"��9�˳����ѡ��ģʽ";
	Json::Value jv4;
	jv4["text"] = u8"��9��س���";
	
	Json::Value jv5;
	jv5["text"] = u8"��9��ѯ���������Ϣ";

	Json::Value jv6;
	jv6["text"] = u8"��9������ӵ�е����";
	Json::Value jv7;
	jv7["text"] = u8"��9����Ȩ������";

	jv["buttons"].append(jv2);
	jv["buttons"].append(jv3);
	jv["buttons"].append(jv4);
	jv["buttons"].append(jv5);
	jv["buttons"].append(jv6);
	jv["buttons"].append(jv7);

	return sendForm(xuid, jv.toStyledString());
}
// ��ع���gui
static UINT send_landbuygui(string xuid, string title, string content, string button1, string button2) {
	Player* p = onlinePlayers[xuid];
	std::string str = createModalFormString(title, content, button1, button2);
	return sendForm(xuid, str);
}
// ��س���gui
static UINT send_landsellgui(string xuid, string title, string content, string button1, string button2) {
	Player* p = onlinePlayers[xuid];
	std::string str = createModalFormString(title, content, button1, button2);
	return sendForm(xuid, str);
}
// �������gui
static UINT send_landshare_gui(string xuid) {
	Json::Value jv;
	jv["type"] = "custom_form";
	jv["title"] = u8"��9��ط���";
	jv["content"];

	Json::Value jv2;
	jv2["type"] = "dropdown";
	jv2["text"] = u8"��e��ѡ��";
	jv2["options"].append(u8"����");
	jv2["options"].append(u8"ȡ������");

	Json::Value jv3;
	jv3["type"] = "dropdown";
	jv3["text"] = u8"��e��ѡ����ң�";
	map<string, string>::iterator it;
	Json::Value name;
	name["name"];
	int i = 0;
	for (it = playerName.begin(); it != playerName.end(); ++it) {
		char ii[64];
		sprintf_s(ii, "%d]", i);
		name["name"][ii];
		name["name"][ii] = it->second;
		jv3["options"].append(it->second);
		i++;
	}
	string content = PLAYERCONFIG;
	Json::Reader reader;
	Json::Value root;
	if (reader.parse(content, root)) {
		const Json::Value arrayObj3 = root["limit"][xuid]["share_name"];
		int length3 = arrayObj3.size();
		if (length3 > 0) {
			for (int i3 = 0; i3 < length3; i3++)
			{
				string b4 = root["limit"][xuid]["share_name"][i3].asString();
				char ii[64];
				sprintf_s(ii, "%d]", i);
				name["name"][ii];
				name["name"][ii] = b4;
				jv3["options"].append(b4);
				i++;
			}
		}
	}
	jv["content"].append(jv2);
	jv["content"].append(jv3);

	Json::FastWriter sw;

	gui_land_sharename[xuid] = (sw.write(name));

	return sendForm(xuid, jv.toStyledString());
}
// ���Ȩ��gui
static UINT send_landpermission_gui(string xuid) {
	Json::Value jv;
	jv["type"] = "custom_form";
	jv["title"] = u8"��9���Ȩ������";
	jv["content"];

	bool putblock; bool destroyblock; bool useitem; bool openchest; bool attack;
	get_player_json(xuid, &putblock, &destroyblock, &useitem, &openchest, &attack);

	Json::Value jv2;
	jv2["type"] = "toggle";
	jv2["text"] = u8"��9���÷���Ȩ�ޣ�";
	jv2["default"] = putblock;

	Json::Value jv3;
	jv3["type"] = "toggle";
	jv3["text"] = u8"��9�ƻ�����Ȩ�ޣ�";
	jv3["default"] = destroyblock;

	Json::Value jv4;
	jv4["type"] = "toggle";
	jv4["text"] = u8"��9ʹ����ƷȨ�ޣ�";
	jv4["default"] = useitem;

	Json::Value jv5;
	jv5["type"] = "toggle";
	jv5["text"] = u8"��9������Ȩ�ޣ�\n��1(���ȿ���ʹ����ƷȨ��)";
	jv5["default"] = openchest;

	Json::Value jv6;
	jv6["type"] = "toggle";
	jv6["text"] = u8"��9����Ȩ�ޣ�";
	jv6["default"] = attack;

	jv["content"].append(jv2);
	jv["content"].append(jv3);
	jv["content"].append(jv4);
	jv["content"].append(jv5);
	jv["content"].append(jv6);

	return sendForm(xuid, jv.toStyledString());
}
// �����Ϣgui
void send_landinfo_gui(string xuid, string point, string landplayeruid,string name, string share_name) {
	Json::Value jv;
	jv["type"] = "form";
	jv["title"] = u8"��9�����Ϣ";
	string str = u8"��d��ط�Χ: ";
	str.append(point);
	str.append(u8"\n��d�������: ");
	str.append(name);
	str.append(u8"\n��d�������xuid: ");
	str.append(landplayeruid);
	str.append(u8"\n��d�����������: ");
	str.append(share_name);

	jv["content"] = str;
	jv["buttons"];
	Json::Value jv2;
	jv2["text"] = u8"ȷ��";

	jv["buttons"].append(jv2);

	sendForm(xuid, jv.toStyledString());
}
void land_info(string uid, string point) {
	auto b1 = point.find(".");
	auto b2 = point.rfind(".");
	int x = std::stoi(point.substr(0, b1));
	int y = std::stoi(point.substr(b1 + 1, b2 - b1 - 1));
	int z = std::stoi(point.substr(b2 + 1));

	string landpo = island(x, y, z);
	if (landpo == "0") {
		sendText(uid, u8"��c��Ľ���û����أ�", RAW);
		return;
	}
	string content = PLAYERCONFIG;
	string landop = get_land_json(landpo, "xuid");
	string share_name;

	Json::Reader reader;
	Json::Value root;
	if (reader.parse(content, root)) {
		const Json::Value arrayObj3 = root["limit"][landop]["share_name"];
		int length3 = arrayObj3.size();
		if (length3 > 0) {
			for (int i3 = 0; i3 < length3; i3++)
			{
				string b4 = root["limit"][landop]["share_name"][i3].asString();
				share_name.append(b4);
				share_name.append(";");
			}
		}
	}
	string name = get_land_json(landpo, "land_use");
	
	send_landinfo_gui(uid, landpo,landop, name, share_name);
}
//�ʴ�ʽgui
std::string createModalFormString(std::string title, std::string content, std::string button1, std::string button2) {
	Json::Value jv;
	jv["type"] = "modal";
	jv["title"] = title;
	jv["content"] = content;
	jv["button1"] = button1;
	jv["button2"] = button2;
	return jv.toStyledString();
}
//==================== �¼����أ�THook�� =======================

THook(void*, MSSYM_B2QQE170ServerScoreboardB2AAA4QEAAB1AE24VCommandSoftEnumRegistryB2AAE16PEAVLevelStorageB3AAAA1Z, void* _this, void* a2, void* a3) {
	landmoney_scoreboard = (Scoreboard*)original(_this, a2, a3);
	return landmoney_scoreboard;
}
// ��ȡָ�����
THook(VA, MSSYM_MD5_3b8fb7204bf8294ee636ba7272eec000,
	VA _this) {
	p_spscqueue = original(_this);
	return p_spscqueue;
}
// �����������
THook(void, MSSYM_B1QA6handleB1AE20ServerNetworkHandlerB2AAE26UEAAXAEBVNetworkIdentifierB2AAE24AEBVCommandRequestPacketB3AAAA1Z,
	VA _this, VA id, CommandRequestPacket* crp) {
	Player* pPlayer = SYMCALL(Player*, MSSYM_B2QUE15getServerPlayerB1AE20ServerNetworkHandlerB2AAE20AEAAPEAVServerPlayerB2AAE21AEBVNetworkIdentifierB2AAA1EB1AA1Z,
		_this, id, *((char*)crp + 16));

	string uid = pPlayer->getXuid(pxuid_level);
	auto cmd = crp->toString();
	auto dimensionId = pPlayer->getDimensionId();
	//cout << cmd << endl;
	if (cmd == "/land") {
		if (dimensionId == 0) {
			gui_helpfid[uid] = landhelp_gui(uid);
		}
		else {
			TextType tp{ RAW };
			sendText(uid, u8"��cֻ����������ʹ�����ϵͳ", tp);
		}
		return;
	}
	if (cmd == "/landopen") {
		if (dimensionId == 0) {
			playerOpen[uid] = true;

			TextType tp{ RAW };
			sendText(uid, u8"��a�ѿ������ѡ��ģʽ\n��b���÷����ѡ���A\n��e���ֵ��������˳����ѡ��ģʽ", tp);
		}
		else {
			TextType tp{ RAW };
			sendText(uid, u8"��cֻ����������ʹ�����ϵͳ", tp);
		}
		return;
	}
	if (cmd == "/landoff") {
		if (dimensionId == 0) {
			TextType tp{ RAW };
			if (playerOpen[uid]) {
				playerOpen.erase(uid);
				playerSetA[uid] = "0";
				playerSetB[uid] = "0";

				sendText(uid, u8"��c�ѹر����ѡ��ģʽ", tp);
			}
			else {
				sendText(uid, u8"��cδ�������ѡ��ģʽ", tp);
			}
		}
		else {
			TextType tp{ RAW };
			sendText(uid, u8"��cֻ����������ʹ�����ϵͳ", tp);
		}
		return;
	}
	if (cmd == "/landreload") {
		TextType tp{ RAW };
		sendText(uid, u8"��c���ڿ���̨���������", tp);
		return;
	}
	if (cmd == "/landsell") {
		int money;
		string landpo;
		if (land_sell(uid, pPlayer->getPos()->toJsonString(), &money,&landpo)) {
			string str2 = u8"��6��ȷ�����Ƿ�Ҫ���۽������: ";
			str2.append(landpo);
			str2.append(u8"\n��e����ý��: ");
			char aa[128];
			sprintf_s(aa, u8"%d", money);
			str2.append(aa);

			gui_sellfid[uid] = send_landsellgui(uid, u8"��b�Ƿ���۸����", str2, u8"ȷ��", u8"ȡ��");
		}
		else {
			sendText(uid, u8"��c����ʧ�ܣ�", RAW);
		}
		return;
	}
	original(_this, id, crp);
}
// ����̨����
THook(bool, MSSYM_MD5_b5c9e566146b3136e6fb37f0c080d91e,
	VA _this, std::string* cmd) {
	if (*cmd == "landreload") {
		cout << u8"�������������ļ���.." << endl;
		LCONFIG = getjson(".\\land\\config.json");
		LANDCONFIG = getjson(".\\land\\land.json");
		PLAYERCONFIG = getjson(".\\land\\player.json");
		return true;
	}
	return original(_this, cmd);
}
// ���ʹ����Ʒ
THook(bool, MSSYM_B1QA9useItemOnB1AA8GameModeB2AAA4UEAAB1UE14NAEAVItemStackB2AAE12AEBVBlockPosB2AAA9EAEBVVec3B2AAA9PEBVBlockB3AAAA1Z,
	void* _this, ItemStack* item, BlockPos* pBlkpos, unsigned __int8 a4, void* v5, Block* pBlk) {

	auto pPlayer = *reinterpret_cast<Player**>(reinterpret_cast<VA>(_this) + 8);
	if (player_isoline[pPlayer]) {
		auto position = pBlkpos->getPosition()->toJsonString();

		auto itemid = item->getId();
		auto itemaux = item->getAuxValue();
		auto itemname = item->getName();
		auto dimensionId = pPlayer->getDimensionId();

		string uid = pPlayer->getXuid(pxuid_level);
		auto dimid = pPlayer->getDimensionId();
		bool isid = isitemid(itemid, itemaux);

		if (isid && itemid != 0) {
			if (dimid == 0) {
				gui_helpfid[uid] = landhelp_gui(uid);
			}
			else { sendText(uid, u8"��c���ϵͳֻ����������ʹ�ã�", RAW); }
		}
		if (dimid == 0) {

			if (playerOpen[uid] && itemid == 0) {
				playerOpen.erase(uid);
				playerSetA[uid] = "0";
				playerSetB[uid] = "0";

				string say = u8"��c�˳����ѡ��ģʽ";

				sendText(uid, say, RAW);
				return 0;
			}

			if (!island_player(uid, pPlayer->getNameTag(), position, USEITEM)) {
				sendText(uid, u8"��c[���ϵͳ]��û�и����ʹ����Ʒ��Ȩ�ޣ�", RAW);
				return 0;
			}
		}
	}
	return original(_this, item, pBlkpos, a4, v5, pBlk);

}
// ��ҷ��÷���
THook(bool,MSSYM_B1QA8mayPlaceB1AE11BlockSourceB2AAA4QEAAB1UE10NAEBVBlockB2AAE12AEBVBlockPosB2AAE10EPEAVActorB3AAUA1NB1AA1Z,
	BlockSource* class_this, Block* pBlk, BlockPos* pBlkpos, unsigned __int8 a4, Player* pPlayer, bool _bool) {

	auto position = pBlkpos->getPosition()->toJsonString();
	
	if (player_isoline[pPlayer]) {
		string uid = pPlayer->getXuid(pxuid_level);
		auto b1 = position.find(".");
		auto b2 = position.rfind(".");
		int x = std::stoi(position.substr(0, b1));
		int y = std::stoi(position.substr(b1 + 1, b2 - b1 - 1));
		int z = std::stoi(position.substr(b2 + 1));

		if ((pPlayer->getDimensionId() == 0)) {
			
			if (playerOpen[uid]) {
				playerSetA[uid] = position;

				string say = u8"��b�������õ�A��";
				say.append(playerSetA[uid]);
				say.append(u8"\n��d�ƻ���������õ�B��\n��e���ֵ��������˳����ѡ��ģʽ��");

				sendText(uid, say, RAW);
				return 0;
			}
			if (!island_player(uid, pPlayer->getNameTag(), position, PUTBLOCK)) {
				sendText(uid, u8"��c[���ϵͳ]��û�и���ط��÷����Ȩ�ޣ�", RAW);
				return 0;
			}
		}
	}
	return original( class_this,pBlk,pBlkpos,a4,pPlayer,_bool);
}
// ����ƻ�����
THook(bool, MSSYM_B2QUE20destroyBlockInternalB1AA8GameModeB2AAA4AEAAB1UE13NAEBVBlockPosB2AAA1EB1AA1Z,
	void* _this, BlockPos* pBlkpos) {

	Player* pPlayer = *reinterpret_cast<Player**>(reinterpret_cast<VA>(_this) + 8);
	//const Block* pBlk = (*(BlockSource**)(*((__int64*)_this + 1) + 840i64))->getBlock(pBlkpos);
	if (player_isoline[pPlayer]) {
		auto position = pBlkpos->getPosition()->toJsonString();

		string uid = pPlayer->getXuid(pxuid_level);
		//cout << uid << endl;
		if (pPlayer->getDimensionId() == 0) {
			TextType tp{ RAW };
			if (playerOpen[uid]) {
				if (playerSetA[uid] == "0" || !playerSetA.count(uid)) {
					sendText(uid, u8"��c����ʧ�ܣ��������õ�A��\n��e���ֵ��������˳����ѡ��ģʽ��", tp);
					return false;
				}
				else {
					playerSetB[uid] = position;
					//string say = u8"��b�������õ�B��";
					//say.append(playerSetB[uid]);
					//say.append(u8"\n��e���ֵ��������˳����ѡ��ģʽ��");

					string poa = playerSetA[uid];
					string pob = playerSetB[uid];
					if (poa == "0" || pob == "0") {
						sendText(uid, u8"��c����ʧ�ܣ�������������\n��e���ֵ��������˳����ѡ��ģʽ��", tp);
						return false;
					}
					auto b1 = poa.find(".");
					auto b2 = poa.rfind(".");
					int x1 = std::stoi(poa.substr(0, b1));
					int y1 = std::stoi(poa.substr(b1 + 1, b2 - b1 - 1));
					int z1 = std::stoi(poa.substr(b2 + 1));

					auto b3 = pob.find(".");
					auto b4 = pob.rfind(".");
					int x2 = std::stoi(pob.substr(0, b3));
					int y2 = std::stoi(pob.substr(b3 + 1, b4 - b3 - 1));
					int z2 = std::stoi(pob.substr(b4 + 1));

					int dx = abs(abs(x1) - abs(x2)) + 1;
					int dz = abs(abs(z1) - abs(z2)) + 1;

					int s = dx * dz;
					int money = s * (get_landmoney(true));

					string str = u8"��6��A: ";
					str.append(poa);
					str.append(u8"\n��6��B: ");
					str.append(pob);
					char str2[512];
					sprintf_s(str2, u8"\n��d�����: %d \n��e��Ҫ���: %d\n��e���н��: %d", s, money, getmoney(uid));
					str.append(str2);

					gui_buyfid[uid] = send_landbuygui(uid, u8"��b�Ƿ������", str, u8"��������", u8"����ѡ�����");
					//sendText(uid, say, RAW);
				}
				return false;
			}
			if (!island_player(uid, pPlayer->getNameTag(), position, DESTROYBLOCK)) {
				sendText(uid, u8"��c[���ϵͳ]��û�и�����ƻ������Ȩ��", tp);
				return false;
			}
		}
	}
	return original(_this, pBlkpos);
}
// ��ҿ���
THook(bool, MSSYM_B1QA3useB1AE10ChestBlockB2AAA4UEBAB1UE11NAEAVPlayerB2AAE12AEBVBlockPosB3AAAA1Z,
	void* _this, Player* pPlayer, BlockPos* pBlkpos) {

	auto position = pBlkpos->getPosition()->toJsonString();

	string uid = pPlayer->getXuid(pxuid_level);


	if (pPlayer->getDimensionId() == 0) {
		if (!island_player(uid, pPlayer->getNameTag(), position, OPENCHEST)) {
			sendText(uid, u8"��c[���ϵͳ]��û�и���ؿ���������Ȩ�ޣ�", RAW);
			return 0;
		}
	}
	return original(_this, pPlayer, pBlkpos);
}
// ��ҿ���¯
THook(bool, MSSYM_B1QA9startOpenB1AE17FurnaceBlockActorB2AAE15UEAAXAEAVPlayerB3AAAA1Z,
	void* _this, Player* pPlayer, BlockPos* pBlkpos) {

	auto position = pBlkpos->getPosition()->toJsonString();

	string uid = pPlayer->getXuid(pxuid_level);

	if (pPlayer->getDimensionId() == 0) {
		if (!island_player(uid, pPlayer->getNameTag(), position, OPENCHEST)) {
			sendText(uid, u8"��c[���ϵͳ]��û�и���ؿ���������Ȩ�ޣ�", RAW);
			return 0;
		}
	}
	return original(_this, pPlayer, pBlkpos);
}
// ��ҿ�Ͱ
THook(bool, MSSYM_B1QA3useB1AE11BarrelBlockB2AAA4UEBAB1UE11NAEAVPlayerB2AAE12AEBVBlockPosB3AAAA1Z,
	void* _this, Player* pPlayer, BlockPos* pBlkpos) {
	auto position = pBlkpos->getPosition()->toJsonString();
	string uid = pPlayer->getXuid(pxuid_level);

	if (pPlayer->getDimensionId() == 0) {

		if (!island_player(uid, pPlayer->getNameTag(), position, OPENCHEST)) {
			sendText(uid, u8"��c[���ϵͳ]��û�и���ؿ���������Ȩ�ޣ�", RAW);
			return 0;
		}
	}
	return original(_this, pPlayer, pBlkpos);
}
// ��ҹ���ʱ��������
THook(bool, MSSYM_B1QA6attackB1AA6PlayerB2AAA4UEAAB1UE10NAEAVActorB3AAAA1Z,
	Player* class_this, Actor* pactor) {
	//std::string p_player_name = class_this->getNameTag();
	//std::string p_actor_name = pactor->getNameTag();
	//std::string actor_typename = pactor->getTypeName();
	if (player_isoline[class_this]) {
		//Vec3* p_position = pactor->getPos();

		//auto actorname = p_actor_name;
	//	auto actortype = actor_typename;
		auto position = pactor->getPos()->toJsonString();

		string uid = class_this->getXuid(pxuid_level);

		if (class_this->getDimensionId() == 0) {
			if (!island_player(uid, class_this->getNameTag(), position, ATTACKT)) {
				sendText(uid, u8"��c[���ϵͳ]��û�и���صĹ���Ȩ�ޣ�", RAW);
				return false;
			}
		}
	}
    return original(class_this, pactor);
}
// ����л�ά��
THook(bool, MSSYM_B2QUE21playerChangeDimensionB1AA5LevelB2AAA4AEAAB1UE11NPEAVPlayerB2AAE26AEAVChangeDimensionRequestB3AAAA1Z,
	void* _this, Player* pPlayer, void* req) {
	if (player_isoline[pPlayer]) {
		string uid = pPlayer->getXuid(pxuid_level);
		if (playerOpen[uid]) {
			playerOpen[uid] = false;
			playerSetA[uid] = "0";
			playerSetB[uid] = "0";

			TextType tp{ RAW };
			sendText(uid, u8"��c�л�ά�Ƚ��ر����ѡ��ģʽ", tp);
		}
	}
	return original(_this, pPlayer, req);
}
// ����뿪��Ϸ
THook(void, MSSYM_B2QUE12onPlayerLeftB1AE20ServerNetworkHandlerB2AAE21AEAAXPEAVServerPlayerB3AAUA1NB1AA1Z,
	VA _this, Player* pPlayer, char v3) {
	auto playername = pPlayer->getNameTag();
	string uid = pPlayer->getXuid(pxuid_level);
	playerOpen.erase(uid);
	playerName.erase(uid);
	player_isoline.erase(pPlayer);

	onlinePlayers.erase(uid);
	playerSetA.erase(uid);
	playerSetB.erase(uid);

	gui_buyfid.erase(uid);
	gui_sellfid.erase(uid);
	gui_helpfid.erase(uid);

	gui_landsharefid.erase(uid);
	gui_landpermissionfid.erase(uid);
    gui_land_sharename.erase(uid);

	original(_this, pPlayer, v3);
}
// ��Ҽ���
THook(void, MSSYM_B1QE14onPlayerJoinedB1AE16ServerScoreboardB2AAE15UEAAXAEBVPlayerB3AAAA1Z, VA* class_this, Player* player) {
	std::string playername = player->getNameTag();

	string uid = player->getXuid(pxuid_level);

	playerOpen[uid] = false;
	playerName[uid] = playername;
	player_isoline[player] = true;

	playerSetA[uid] = "0";
	playerSetB[uid] = "0";

	onlinePlayers[uid] = player;
	cout << u8"[��ز��]��� " << playername << u8" ������Ϸ,Xuid:" << uid << endl;
	original(class_this, player);
}
// ����ע��
THook(void, MSSYM_B1QA5setupB1AE20ChangeSettingCommandB2AAE22SAXAEAVCommandRegistryB3AAAA1Z, CommandRegistry* _this) {
	_this->registerCommand("land", u8"�ٻ���ز˵�", 0, { 0 }, { 0x40 });
	_this->registerCommand("landsell", u8"���۽������", 0, { 0 }, { 0x40 });
	_this->registerCommand("landopen", u8"�����ѡ��ģʽ", 0, { 0 }, { 0x40 });
	_this->registerCommand("landoff", u8"�ر����ѡ��ģʽ", 0, { 0 }, { 0x40 });
	_this->registerCommand("landreload", u8"����������������ļ�", 0, { 0 }, { 0x40 });
	original(_this);
}

