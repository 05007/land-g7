#pragma once
#include"预编译头.h"

// 方块坐标结构体
struct BPos3 {
	INT32 x;
	INT32 y;
	INT32 z;
	std::string toJsonString() {
		char str[256];
		sprintf_s(str, "%d.%d.%d", x, y, z);//领地格式x.y.z
		return std::string(str);
	}
};
struct BlockPos {
	// 获取坐标数组头
	BPos3* getPosition() const {
		return reinterpret_cast<BPos3*>(reinterpret_cast<VA>(this));
	}
};

struct Block {
	
};

struct BlockActor {
	// 取方块
	Block* getBlock() {
		return *reinterpret_cast<Block**>(reinterpret_cast<VA>(this) + 16);
	}
	// 取方块位置
	BlockPos* getPosition() {				// IDA BlockActor::BlockActor
		return reinterpret_cast<BlockPos*>(reinterpret_cast<VA>(this) + 44);
	}
};

struct BlockSource {
	// 取方块
	Block* getBlock(const BlockPos* blkpos) {
		return SYMCALL(Block*,
			MSSYM_B1QA8getBlockB1AE11BlockSourceB2AAE13QEBAAEBVBlockB2AAE12AEBVBlockPosB3AAAA1Z,
			this, blkpos);
	}
};

struct MCUUID {
	// 取uuid字符串
	std::string toString() {
		std::string s;
		SYMCALL(std::string&, MSSYM_MD5_40e8abf6eb08f7ee446159cdd0a7f283, this, &s);
		return s;
	}
};

// 玩家坐标结构体
struct Vec3 {
	float x;
	float y;
	float z;

	std::string toJsonString() {
		char str[256];
		sprintf_s(str, "%d.%d.%d", (int)x, (int)y, (int)z);
		return std::string(str);
	}
};

struct Actor {
	// 取方块源
	BlockSource* getRegion() {
		return *reinterpret_cast<BlockSource**>(reinterpret_cast<VA>(this) + 414 * sizeof(void*));
	}

	// 获取生物名称信息
	std::string getNameTag() {
		return SYMCALL(std::string&,
			MSSYM_MD5_7044ab83168b0fd345329e6566fd47fd,
			this);
	}

	// 获取生物当前所处维度ID
	int getDimensionId() {
		int dimensionId;
		SYMCALL(int&,
			MSSYM_B1QE14getDimensionIdB1AA5ActorB2AAA4UEBAB1QA2AVB2QDE11AutomaticIDB1AE10VDimensionB2AAA1HB2AAA2XZ,
			this, &dimensionId);
		return dimensionId;
	}


	// 获取生物当前所在坐标
	Vec3* getPos() {
		return SYMCALL(Vec3*,
			MSSYM_B1QA6getPosB1AA5ActorB2AAE12UEBAAEBVVec3B2AAA2XZ, this);
	}

	// 获取生物类型
	std::string getTypeName() {
		std::string actor_typename;
		SYMCALL(std::string&,
			MSSYM_MD5_01064f7d893d9f9ef50acf1f931d1d79,
			&actor_typename, this);
		return actor_typename;
	}
};
struct Mob : Actor {
};
struct Player : Actor {
	// 取uuid
	MCUUID* getUuid() {				// IDA ServerNetworkHandler::_createNewPlayer//185行，跟踪asString
		return (MCUUID*)((VA)this + 2720);
	}

    std::string getXuid(VA level) {
        return SYMCALL(std::string&, MSSYM_MD5_337bfad553c289ba4656ac43dcb60748,
            level, (VA)this + 2720);
    }
};

struct ItemStack {
	// 取物品ID
	short getId() {
		return SYMCALL(short,
			MSSYM_B1QA5getIdB1AE13ItemStackBaseB2AAA7QEBAFXZ,
			this);
	}
	// 取物品特殊值
	short getAuxValue() {
		return SYMCALL(short,
			MSSYM_B1QE11getAuxValueB1AE13ItemStackBaseB2AAA7QEBAFXZ,
			this);
	}
	// 取物品名称
	std::string getName() {
		std::string str;
		SYMCALL(__int64,
			MSSYM_MD5_6d581a35d7ad70fd364b60c3ebe93394,
			this, &str);
		return str;
	}
	// 判断是否空容器
	bool isNull() {
		return SYMCALL(bool,
			MSSYM_B1QA6isNullB1AE13ItemStackBaseB2AAA4QEBAB1UA3NXZ,
			this);
	}
};

struct ContainerItemStack
	:ItemStack {

};

struct CommandRequestPacket {
	// 取命令文本
	std::string toString() {			// IDA ServerNetworkHandler::handle
		std::string str = std::string(*(std::string*)((VA)this + 40));
		return str;
	}
};

struct ModalFormResponsePacket {
    //取发起表单ID
	UINT getFormId() {
		return *(UINT*)((VA)this + 40);
	}

	// 取选择序号
	std::string getSelectStr() {
		std::string x = *(std::string*)((VA)this + 48);
		auto l = x.length();
		if (x.c_str()[l - 1] == '\n') {
			return l > 1 ? x.substr(0, l - 1) :
				x;
		}
		return x;
	}
};
//===========bdx===============
#ifdef BUF_CHK
#define DO_BUF_CHK() assert(datamax > data)
#define BUF_CHK_VAR uintptr_t datamax
#else
#define DO_BUF_CHK()
#define BUF_CHK_VAR
#endif

using std::string, std::string_view;
static inline string ifs2str(std::ifstream& ifs) {
    return { (std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>() };
}
using std::string, std::string_view;
template <class T>
struct is_safe_obj : std::integral_constant<bool,
    !std::is_class<T>::value>
{
};

typedef unsigned int bsize_t;
class RBStream
{
public:
    uintptr_t data;
    BUF_CHK_VAR;
    RBStream(void* dat, size_t len)
    {
        data = (uintptr_t)dat;
#ifdef BUF_CHK
        datamax = data;
        datamax += len;
#endif
    }
    RBStream(string_view x) noexcept {
        *this = { (void*)x.data(), (size_t)x.size() };
    }

private:
    template<typename T1, typename T2>
    void __get(std::unordered_map<T1, T2>& x) {
        bsize_t sz;
        __get(sz);
        x.reserve(sz);
        for (bsize_t i = 0; i < sz; ++i)
        {
            T1 local;
            T2 local2;
            __get(local);
            __get(local2);
            x.emplace(local, local2);
        }
    }
    template <typename T1>
    void __get(std::vector<T1>& x)
    {
        bsize_t sz;
        __get(sz);
        x.reserve(sz);
        for (bsize_t i = 0; i < sz; ++i)
        {
            T1 local;
            __get(local);
            x.push_back(local);
        }
    }
    template <typename T1>
    void __get(std::list<T1>& x)
    {
        bsize_t sz;
        __get(sz);
        for (bsize_t i = 0; i < sz; ++i)
        {
            T1 local;
            __get(local);
            x.push_back(local);
        }
    }
    void __get(string& x)
    {
        bsize_t sz;
        __get(sz);
        x.reserve(sz);
        x.append((const char*)data, sz);
        data += sz;
        DO_BUF_CHK();
    }
    template <typename T>
    void __get(T& x)
    {
        if constexpr (is_safe_obj<T>())
        {
            static_assert(!std::is_reference<T>());
            memcpy(&x, (void*)data, sizeof(x));
            data += sizeof(T);
            DO_BUF_CHK();
        }
        else
        {
            x.unpack(*this);
        }
    }

public:
    template <typename... T>
    void apply(T&&... args) noexcept
    {
        (__get(std::forward<T>(args)), ...);
    }
    void read(void* dst, size_t n)
    {
        memcpy(dst, (void*)data, n);
        data += n;
        DO_BUF_CHK();
    }
};
template<typename container>
class WBStreamImpl
{
public:
    container data;

private:
    template<typename T1, typename T2>
    void __put(const std::unordered_map<T1, T2>& x) {
        bsize_t sz = x.size();
        __put(sz);
        for (auto& [k, v] : x) {
            __put(k);
            __put(v);
        }
    }
    template <typename T2>
    void __put(const std::vector<T2>& x)
    {
        bsize_t sz = x.size();
        __put(sz);
        for (auto i = x.begin(); i != x.end(); ++i)
        {
            __put(*i);
        }
    }
    template <typename T2>
    void __put(const std::list<T2>& x)
    {
        bsize_t sz = x.size();
        __put(sz);
        for (auto i = x.begin(); i != x.end(); ++i)
        {
            __put(*i);
        }
    }
    void __put(const string& x)
    {
        __put((bsize_t)x.size());
        data.append(x);
    }
    void __put(const string_view x)
    {
        __put((bsize_t)x.size());
        data.append(x);
    }
    template <typename T>
    void __put(T&& x)
    {
        if constexpr (is_safe_obj<T>())
        {
            data.append((const char*)&x, sizeof(T));
        }
        else
        {
            x.pack(*this);
        }
    }

public:
    WBStreamImpl() {}
    WBStreamImpl(container&& x) :data(x) {}
    template <typename... T>
    void apply(T&&... args) noexcept
    {
        (__put(std::forward<T>(args)), ...);
    }
    void write(const void* src, size_t n)
    {
        data.append((const char*)src, n);
    }
    operator string_view() {
        return data;
    }
};
using WBStream = WBStreamImpl<std::string>;
template<typename T>
static inline auto S(T&& x) {
    using DT = std::decay_t<T>;
    if constexpr (!(std::is_same_v<DT, string> || std::is_same_v<DT, string_view> || std::is_same_v<DT, const char*>))
        return std::to_string(std::forward<T>(x));
    else return x;
}
template<typename T>
static inline auto QUOTE(T&& x) {
    if (x.size() && x[0] == '"') return x;
    if constexpr (std::is_same_v<std::decay_t<T>, string>)
        return '"' + x + '"';
    else
        return '"' + string(x) + '"';
}
typedef int s32;
typedef int64_t s64;
typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

using std::string_view;
template <typename T, int off>
inline T& dAccess(void* ptr) {
    return *(T*)(((uintptr_t)ptr) + off);
}
template <typename T, int off>
inline T const& dAccess(void const* ptr) {
    return *(T*)(((uintptr_t)ptr) + off);
}
template <typename T>
inline T& dAccess(void* ptr, uintptr_t off) {
    return *(T*)(((uintptr_t)ptr) + off);
}
template <typename T>
inline const T& dAccess(void const* ptr, uintptr_t off) {
    return *(T*)(((uintptr_t)ptr) + off);
}
#define __WEAK __declspec(selectany)

class BinaryStream {
public:
    std::string& getRaw() {
        return *dAccess<std::string*, 88>(this);
    }
};
class Packet {
public:
    u32 unk2 = 2;                                 // 8
    u32 reliableOrdered = 1; // 12
    u8 clientSubId = 0;                                 // 16
    u64 unk24_0 = 0; //24
    u32 compressible = 0;                                // 32
    inline Packet(unsigned compress) : compressible(compress) {}
    inline Packet() {}
    inline virtual ~Packet() {}
    virtual int getId() const = 0;
    virtual std::string getName() const = 0;
    virtual void write(BinaryStream&) const = 0;
    virtual void dummyread() = 0;
    virtual bool disallowBatching() const = 0;
};
static_assert(sizeof(Packet) == 40);

template <int pid, bool batching = true, bool compress = true>
class MyPkt :public Packet {
public:
    string_view view;
    MyPkt() {
        compressible = compress;
    }
    MyPkt(string_view sv) :view(sv) {
        compressible = compress;
    }
    inline virtual ~MyPkt() {}
    virtual int getId() const {
        return pid;
    }
    virtual std::string getName() const {
        return "MyPkt";
    }

    virtual void write(BinaryStream& bs) const {
        bs.getRaw().append(view);
    }
    virtual void dummyread() {}
    virtual bool disallowBatching() const { return !batching; }
};
static_assert(offsetof(MyPkt<0>, view) == 40);

enum TextType : char {
    RAW = 0,
    POPUP = 3,
    JUKEBOX_POPUP = 4,
    TIP = 5
};

template <typename T>
struct VarInts {
    T v;
    operator T() {
        return v;
    }
    VarInts() {}
    VarInts(T x) { v = x; }
    template<typename _TP>
    void pack(WBStreamImpl<_TP>& ws) const {
        unsigned char buf[16];
        int ptr = 0;
        T enc = v;
        do {
            buf[ptr++] = enc & 0x7f;
            enc >>= 7;
        } while (enc);
        for (int i = 0; i < ptr - 1; ++i) {
            buf[i] |= 0x80;
        }
        ws.write(buf, ptr);
    }
    void unpack(RBStream& rs) {
        unsigned char buf[16];
        v = 0;
        int ptr = 0;
        for (; ptr < 16; ++ptr) {
            rs.apply(buf[ptr]);
            if (!(buf[ptr] & 0x80)) {
                ++ptr;
                break;
            }
            buf[ptr] &= 0x7f;
        }
        for (ptr = ptr - 1; ptr >= 0; --ptr) {
            v <<= 7;
            v |= buf[ptr];
        }
    }
};
using VarUInt = VarInts<unsigned int>;
using VarULong = VarInts<unsigned long long>;

struct MCString {
    string_view view;
    MCString() {}
    MCString(string_view sv) : view(sv) {}
    template <typename T>
    void pack(T& ws) const {
        ws.apply(VarUInt((unsigned int)view.size()));
        ws.write(view.data(), view.size());
    }
    void unpack(RBStream& rs) {
        VarUInt sz;
        rs.apply(sz);
        view = string_view((const char*)rs.data, sz.v);
        rs.data += sz.v;
    }
};
struct ModalFormRequestPacket {
    char filler[0x48];
};
