#include "BasicIn.hpp"
#include "KernelOut.hpp"
#include "PS2.hpp"
#include "memory.hpp"
#include "serial.hpp"

namespace Kernel {

static uint8_t usedMode;
typedef void (*Init_FNT)();
typedef uint8_t (*ReadB_FNT)();
typedef bool (*IsEmpty_FNT)();

static void initSerial();
static void initPs2();
static void initUsbKeyboard();
static constexpr Init_FNT initFunctions[]{nullptr, initSerial, initPs2, initUsbKeyboard};

static uint8_t readBlockingSerial();
static uint8_t readBlockingPs2();
static uint8_t readBlockingUsbKeyboard();
static constexpr ReadB_FNT readBFunctions[]{nullptr, readBlockingSerial, readBlockingPs2, readBlockingUsbKeyboard};

static bool isEmptySerial();
static bool isEmptyPs2();
static bool isEmptyUsbKeyboard();
static constexpr IsEmpty_FNT isEmptyFunctions[]{nullptr, isEmptySerial, isEmptyPs2, isEmptyUsbKeyboard};

static_assert(sizeof(initFunctions) / sizeof(Init_FNT) == sizeof(readBFunctions) / sizeof(ReadB_FNT));
static_assert(sizeof(initFunctions) / sizeof(Init_FNT) == sizeof(isEmptyFunctions) / sizeof(IsEmpty_FNT));
static constexpr uint64_t size = sizeof(initFunctions) / sizeof(Init_FNT);

static constexpr uint64_t max(uint64_t a, uint64_t b) {
    return (a > b ? a : b);
}

static uint64_t keyLayout[2048]{
        0, 0, 0, 0, 0, 0, 0, 0,                                                                                                                                        // 0x00
        0x7D3946307B1B, 0x7D3946317B1B, 0x7D3946327B1B, 0x7D3946337B1B, 0x7D3946347B1B, 0x7D3946357B1B, 0x7D3946367B1B, 0x7D3946377B1B,                                // 0x01
        0x7D3546307B1B, 0x7D3546317B1B, 0x7D3546327B1B, 0x7D3546337B1B, 0x7D3546347B1B, 0x7D3546357B1B, 0x7D3546367B1B, 0x7D3546377B1B,                                // 0x02
        0x7D3346307B1B, 0x7D3346317B1B, 0x7D3346327B1B, 0x7D3346337B1B, 0x7D3346347B1B, 0x7D3346357B1B, 0x7D3346367B1B, 0x7D3346377B1B,                                // 0x03
        0x7D3146307B1B, 0x7D3146317B1B, 0x7D3146327B1B, 0x7D3146337B1B, 0x7D3146347B1B, 0x7D3146357B1B, 0x7D3146367B1B, 0x7D3146377B1B,                                // 0x04
        0x7D3246307B1B, 0x7D3246317B1B, 0x7D3246327B1B, 0x7D3246337B1B, 0x7D3246347B1B, 0x7D3246357B1B, 0x7D3246367B1B, 0x7D3246377B1B,                                // 0x05
        0x7D323146307B1B, 0x7D323146317B1B, 0x7D323146327B1B, 0x7D323146337B1B, 0x7D323146347B1B, 0x7D323146357B1B, 0x7D323146367B1B, 0x7D323146377B1B,                // 0x06
        0x7D303146307B1B, 0x7D303146317B1B, 0x7D303146327B1B, 0x7D303146337B1B, 0x7D303146347B1B, 0x7D303146357B1B, 0x7D303146367B1B, 0x7D303146377B1B,                // 0x07
        0x7D3846307B1B, 0x7D3846317B1B, 0x7D3846327B1B, 0x7D3846337B1B, 0x7D3846347B1B, 0x7D3846357B1B, 0x7D3846367B1B, 0x7D3846377B1B,                                // 0x08
        0x7D3646307B1B, 0x7D3646317B1B, 0x7D3646327B1B, 0x7D3646337B1B, 0x7D3646347B1B, 0x7D3646357B1B, 0x7D3646367B1B, 0x7D3646377B1B,                                // 0x09
        0x7D3446307B1B, 0x7D3446317B1B, 0x7D3446327B1B, 0x7D3446337B1B, 0x7D3446347B1B, 0x7D3446357B1B, 0x7D3446367B1B, 0x7D3446377B1B,                                // 0x0A
        0x9, 0x7D9317B1B, 0x7D9327B1B, 0x7D9337B1B, 0x7D9347B1B, 0x7D9357B1B, 0x7D9367B1B, 0x7D9377B1B,                                                                // 0x0B
        0x5E, 0xB0, 0x7D5E327B1B, 0x7D5E337B1B, 0x7D5E347B1B, 0x7D5E357B1B, 0x7D5E367B1B, 0x7D5E377B1B,                                                                // 0x0C
        0x71, 0x51, 0x7D71327B1B, 0x7D71337B1B, 0x7D71347B1B, 0x7D71357B1B, 0x40, 0x7D71377B1B,                                                                        // 0x0D
        0x31, 0x21, 0x7D31327B1B, 0x7D31337B1B, 0x7D31347B1B, 0x7D71357B1B, 0x7D31367B1B, 0x7D31377B1B,                                                                // 0x0E
        0x79, 0x59, 0x7D79327B1B, 0x7D79337B1B, 0x7D79347B1B, 0x7D79357B1B, 0x7D79367B1B, 0x7D79377B1B,                                                                // 0x0F
        0x73, 0x53, 0x7D73327B1B, 0x7D73337B1B, 0x7D73347B1B, 0x7D73357B1B, 0x7D73367B1B, 0x7D73377B1B,                                                                // 0x10
        0x61, 0x41, 0x7D61327B1B, 0x7D61337B1B, 0x7D61347B1B, 0x7D61357B1B, 0x7D61367B1B, 0x7D61377B1B,                                                                // 0x11
        0x77, 0x57, 0x7D77327B1B, 0x7D77337B1B, 0x7D77347B1B, 0x7D77357B1B, 0x7D77367B1B, 0x7D77377B1B,                                                                // 0x12
        0x32, 0x22, 0x7D32327B1B, 0x7D32337B1B, 0x7D32347B1B, 0x7D32357B1B, 0x7D32367B1B, 0x7D32377B1B,                                                                // 0x13
        0x63, 0x43, 0x7D63327B1B, 0x7D63337B1B, 0x7D63347B1B, 0x7D63357B1B, 0x7D63367B1B, 0x7D63377B1B,                                                                // 0x14
        0x78, 0x58, 0x7D78327B1B, 0x7D78337B1B, 0x7D78347B1B, 0x7D78357B1B, 0x7D78367B1B, 0x7D78377B1B,                                                                // 0x15
        0x64, 0x44, 0x7D64327B1B, 0x7D64337B1B, 0x7D64347B1B, 0x7D64357B1B, 0x7D64367B1B, 0x7D64377B1B,                                                                // 0x16
        0x65, 0x45, 0x7D65327B1B, 0x7D65337B1B, 0x7D65347B1B, 0x7D65357B1B, 0x7D65367B1B, 0x7D65377B1B,                                                                // 0x17
        0x34, 0x24, 0x7D34327B1B, 0x7D34337B1B, 0x7D34347B1B, 0x7D34357B1B, 0x7D34367B1B, 0x7D34377B1B,                                                                // 0x18
        0x33, 0x7D33317B1B, 0x7D33327B1B, 0x7D33337B1B, 0x7D33347B1B, 0x7D33357B1B, 0x7D33367B1B, 0x7D33377B1B,                                                        // 0x19
        0x20, 0x3B, 0x7D20327B1B, 0x7D20337B1B, 0x7D20347B1B, 0x7D20357B1B, 0x7D20367B1B, 0x7D20377B1B,                                                                // 0x1A
        0x76, 0x56, 0x7D76327B1B, 0x7D76337B1B, 0x7D76347B1B, 0x7D76357B1B, 0x7D76367B1B, 0x7D76377B1B,                                                                // 0x1B
        0x66, 0x46, 0x7D66327B1B, 0x7D66337B1B, 0x7D66347B1B, 0x7D66357B1B, 0x7D66367B1B, 0x7D66377B1B,                                                                // 0x1C
        0x74, 0x54, 0x7D74327B1B, 0x7D74337B1B, 0x7D74347B1B, 0x7D74357B1B, 0x7D74367B1B, 0x7D74377B1B,                                                                // 0x1D
        0x72, 0x52, 0x7D72327B1B, 0x7D72337B1B, 0x7D72347B1B, 0x7D72357B1B, 0x7D72367B1B, 0x7D72377B1B,                                                                // 0x1E
        0x35, 0x25, 0x7D35327B1B, 0x7D35337B1B, 0x7D35347B1B, 0x7D35357B1B, 0x7D35367B1B, 0x7D35377B1B,                                                                // 0x1F
        0x6E, 0x4E, 0x7D6E327B1B, 0x7D6E337B1B, 0x7D6E347B1B, 0x7D6E357B1B, 0x7D6E367B1B, 0x7D6E377B1B,                                                                // 0x20
        0x62, 0x42, 0x7D62327B1B, 0x7D62337B1B, 0x7D62347B1B, 0x7D62357B1B, 0x7D62367B1B, 0x7D62377B1B,                                                                // 0x21
        0x68, 0x48, 0x7D68327B1B, 0x7D68337B1B, 0x7D68347B1B, 0x7D68357B1B, 0x7D68367B1B, 0x7D68377B1B,                                                                // 0x22
        0x67, 0x47, 0x7D67327B1B, 0x7D67337B1B, 0x7D67347B1B, 0x7D67357B1B, 0x7D67367B1B, 0x7D67377B1B,                                                                // 0x23
        0x7A, 0x5A, 0x7D7A327B1B, 0x7D7A337B1B, 0x7D7A347B1B, 0x7D7A357B1B, 0x7D7A367B1B, 0x7D7A377B1B,                                                                // 0x24
        0x36, 0x26, 0x7D36327B1B, 0x7D36337B1B, 0x7D36347B1B, 0x7D36357B1B, 0x7D36367B1B, 0x7D36377B1B,                                                                // 0x25
        0x6D, 0x4D, 0x7D6D327B1B, 0x7D6D337B1B, 0x7D6D347B1B, 0x7D6D357B1B, 0x7D6D367B1B, 0x7D6D377B1B,                                                                // 0x26
        0x6A, 0x4A, 0x7D6A327B1B, 0x7D6A337B1B, 0x7D6A347B1B, 0x7D6A357B1B, 0x7D6A367B1B, 0x7D6A377B1B,                                                                // 0x27
        0x75, 0x55, 0x7D75327B1B, 0x7D75337B1B, 0x7D75347B1B, 0x7D75357B1B, 0x7D75367B1B, 0x7D75377B1B,                                                                // 0x28
        0x37, 0x2F, 0x7D37327B1B, 0x7D37337B1B, 0x7D37347B1B, 0x7D37357B1B, 0x7B, 0x7D37377B1B,                                                                        // 0x29
        0x38, 0x28, 0x7D38327B1B, 0x7D38337B1B, 0x7D38347B1B, 0x7D38357B1B, 0x5B, 0x7D38377B1B,                                                                        // 0x2A
        0x2C, 0x3B, 0x7D2C327B1B, 0x7D2C337B1B, 0x7D2C347B1B, 0x7D2C357B1B, 0x7D2C367B1B, 0x7D2C377B1B,                                                                // 0x2B
        0x6B, 0x4B, 0x7D6B327B1B, 0x7D6B337B1B, 0x7D6B347B1B, 0x7D6B357B1B, 0x7D6B367B1B, 0x7D6B377B1B,                                                                // 0x2C
        0x69, 0x49, 0x7D69327B1B, 0x7D69337B1B, 0x7D69347B1B, 0x7D69357B1B, 0x7D69367B1B, 0x7D69377B1B,                                                                // 0x2D
        0x6F, 0x4F, 0x7D6F327B1B, 0x7D6F337B1B, 0x7D6F347B1B, 0x7D6F357B1B, 0x7D6F367B1B, 0x7D6F377B1B,                                                                // 0x2E
        0x30, 0x3D, 0x7D30327B1B, 0x7D30337B1B, 0x7D30347B1B, 0x7D30357B1B, 0x7D, 0x7D30377B1B,                                                                        // 0x2F
        0x39, 0x29, 0x7D39327B1B, 0x7D39337B1B, 0x7D39347B1B, 0x7D39357B1B, 0x5D, 0x7D39377B1B,                                                                        // 0x30
        0x2E, 0x3A, 0x7D2E327B1B, 0x7D2E337B1B, 0x7D2E347B1B, 0x7D2E357B1B, 0x7D2E367B1B, 0x7D2E377B1B,                                                                // 0x31
        0x2D, 0x5F, 0x7D2D327B1B, 0x7D2D337B1B, 0x7D2D347B1B, 0x7D2D357B1B, 0x7D2D367B1B, 0x7D62377B332D0,                                                             // 0x32
        0x6C, 0x4C, 0x7D6C327B1B, 0x7D6C337B1B, 0x7D6C347B1B, 0x7D6C357B1B, 0x7D6C367B1B, 0x7D6C377B1B,                                                                // 0x33
        0x7D656F307B1B, 0x7D656F317B1B, 0x7D656F327B1B, 0x7D656F337B1B, 0x7D656F347B1B, 0x7D656F357B1B, 0x7D656F367B1B, 0x7D656F377B1B,                                // 0x34
        0x70, 0x50, 0x7D70327B1B, 0x7D70337B1B, 0x7D70347B1B, 0x7D70357B1B, 0x7D70367B1B, 0x7D70377B1B,                                                                // 0x35
        0x7D3F307B1B, 0x3F, 0x7D3F327B1B, 0x7D3F337B1B, 0x7D3F347B1B, 0x7D3F357B1B, 0x7D3F367B1B, 0x7D3F377B1B,                                                        // 0x36
        0x7D6561307B1B, 0x7D6561317B1B, 0x7D6561327B1B, 0x7D6561337B1B, 0x7D6561347B1B, 0x7D6561357B1B, 0x7D6561367B1B, 0x7D6561377B1B,                                // 0x37
        0x7D6575307B1B, 0x7D6575317B1B, 0x7D6575327B1B, 0x7D6575337B1B, 0x7D6575347B1B, 0x7D6575357B1B, 0x7D6575367B1B, 0x7D6575377B1B,                                // 0x38
        0x7D60307B1B, 0x60, 0x7D60327B1B, 0x7D60337B1B, 0x7D60347B1B, 0x7D60357B1B, 0x7D60367B1B, 0x7D60377B1B,                                                        // 0x39
        0x0A, 0x7D6E5C317B1B, 0x7D6E5C327B1B, 0x7D6E5C337B1B, 0x7D6E5C347B1B, 0x7D6E5C357B1B, 0x7D6E5C367B1B, 0x7D6E5C377B1B,                                          // 0x3A
        0x2B, 0x2A, 0x7D2B327B1B, 0x7D2B337B1B, 0x7D2B347B1B, 0x7D2B357B1B, 0x7E, 0x7D2B377B1B,                                                                        // 0x3B
        0x23, 0x27, 0x7D23327B1B, 0x7D23337B1B, 0x7D23347B1B, 0x7D23357B1B, 0x7D23367B1B, 0x7D23377B1B,                                                                // 0x3C
        0x7D8307B1B, 0x7D8317B1B, 0x7D8327B1B, 0x7D8337B1B, 0x7D8347B1B, 0x7D8357B1B, 0x7D8367B1B, 0x7D8377B1B,                                                        // 0x3D
        0x31, 0x21, 0x7D31327B1B, 0x7D31337B1B, 0x7D31347B1B, 0x7D71357B1B, 0x7D31367B1B, 0x7D31377B1B,                                                                // 0x3E
        0x34, 0x24, 0x7D34327B1B, 0x7D34337B1B, 0x7D34347B1B, 0x7D34357B1B, 0x7D34367B1B, 0x7D34377B1B,                                                                // 0x3F
        0x37, 0x2F, 0x7D37327B1B, 0x7D37337B1B, 0x7D37347B1B, 0x7D37357B1B, 0x7B, 0x7D37377B1B,                                                                        // 0x40
        0x30, 0x3D, 0x7D30327B1B, 0x7D30337B1B, 0x7D30347B1B, 0x7D30357B1B, 0x7D, 0x7D30377B1B,                                                                        // 0x41
        0x2E, 0x3A, 0x7D2E327B1B, 0x7D2E337B1B, 0x7D2E347B1B, 0x7D2E357B1B, 0x7D2E367B1B, 0x7D2E377B1B,                                                                // 0x42
        0x32, 0x22, 0x7D32327B1B, 0x7D32337B1B, 0x7D32347B1B, 0x7D32357B1B, 0x7D32367B1B, 0x7D32377B1B,                                                                // 0x43
        0x35, 0x25, 0x7D35327B1B, 0x7D35337B1B, 0x7D35347B1B, 0x7D35357B1B, 0x7D35367B1B, 0x7D35377B1B,                                                                // 0x44
        0x36, 0x26, 0x7D36327B1B, 0x7D36337B1B, 0x7D36347B1B, 0x7D36357B1B, 0x7D36367B1B, 0x7D36377B1B,                                                                // 0x45
        0x38, 0x28, 0x7D38327B1B, 0x7D38337B1B, 0x7D38347B1B, 0x7D38357B1B, 0x5B, 0x7D38377B1B,                                                                        // 0x46
        0x7D1B307B1B, 0x7D1B317B1B, 0x7D1B327B1B, 0x7D1B337B1B, 0x7D1B347B1B, 0x7D1B357B1B, 0x7D1B367B1B, 0x7D1B377B1B,                                                // 0x47
        0x7D6D754E307B1B, 0x7D6D754E317B1B, 0x7D6D754E327B1B, 0x7D6D754E337B1B, 0x7D6D754E347B1B, 0x7D6D754E357B1B, 0x7D6D754E367B1B, 0x7D6D754E377B1B,                // 0x48
        0x7D313146307B1B, 0x7D313146317B1B, 0x7D313146327B1B, 0x7D313146337B1B, 0x7D313146347B1B, 0x7D313146357B1B, 0x7D313146367B1B, 0x7D313146377B1B,                // 0x49
        0x2B, 0x7D2B317B1B, 0x7D2B327B1B, 0x7D2B337B1B, 0x7D2B347B1B, 0x7D2B357B1B, 0x7D2B367B1B, 0x7D2B377B1B,                                                        // 0x4A
        0x33, 0x7D33317B1B, 0x7D33327B1B, 0x7D33337B1B, 0x7D33347B1B, 0x7D33357B1B, 0x7D33367B1B, 0x7D33377B1B,                                                        // 0x4B
        0x2D, 0x7D2D317B1B, 0x7D2D327B1B, 0x7D2D337B1B, 0x7D2D347B1B, 0x7D2D357B1B, 0x7D2D367B1B, 0x7D2D377B1B,                                                        // 0x4C
        0x2A, 0x7D2A317B1B, 0x7D2A327B1B, 0x7D2A337B1B, 0x7D2A347B1B, 0x7D2A357B1B, 0x7D2A367B1B, 0x7D2A377B1B,                                                        // 0x4D
        0x39, 0x29, 0x7D39327B1B, 0x7D39337B1B, 0x7D39347B1B, 0x7D39357B1B, 0x5D, 0x7D39377B1B,                                                                        // 0x4E
        0x7D6C7273307B1B, 0x7D6C7273317B1B, 0x7D6C7273327B1B, 0x7D6C7273337B1B, 0x7D6C7273347B1B, 0x7D6C7273357B1B, 0x7D6C7273367B1B, 0x7D6C7273377B1B,                // 0x4F
        0x7D3746307B1B, 0x7D3746317B1B, 0x7D3746327B1B, 0x7D3746337B1B, 0x7D3746347B1B, 0x7D3746357B1B, 0x7D3746367B1B, 0x7D3746377B1B,                                // 0x50
        0x7D30306D6D307B1B, 0x7D30306D6D317B1B, 0x7D30306D6D327B1B, 0x7D30306D6D337B1B, 0x7D30306D6D347B1B, 0x7D30306D6D357B1B, 0x7D30306D6D367B1B, 0x7D30306D6D377B1B,// 0x51
        0x7D31306D6D307B1B, 0x7D31306D6D317B1B, 0x7D31306D6D327B1B, 0x7D31306D6D337B1B, 0x7D31306D6D347B1B, 0x7D31306D6D357B1B, 0x7D31306D6D367B1B, 0x7D31306D6D377B1B,// 0x52
        0x7D32306D6D307B1B, 0x7D32306D6D317B1B, 0x7D32306D6D327B1B, 0x7D32306D6D337B1B, 0x7D32306D6D347B1B, 0x7D32306D6D357B1B, 0x7D32306D6D367B1B, 0x7D32306D6D377B1B,// 0x53
        0x7D69756C307B1B, 0x7D69756C317B1B, 0x7D69756C327B1B, 0x7D69756C337B1B, 0x7D69756C347B1B, 0x7D69756C357B1B, 0x7D69756C367B1B, 0x7D69756C377B1B,                // 0x54
        0x7D33306D6D307B1B, 0x7D33306D6D317B1B, 0x7D33306D6D327B1B, 0x7D33306D6D337B1B, 0x7D33306D6D347B1B, 0x7D33306D6D357B1B, 0x7D33306D6D367B1B, 0x7D33306D6D377B1B,// 0x55
        0x7D34306D6D307B1B, 0x7D34306D6D317B1B, 0x7D34306D6D327B1B, 0x7D34306D6D337B1B, 0x7D34306D6D347B1B, 0x7D34306D6D357B1B, 0x7D34306D6D367B1B, 0x7D34306D6D377B1B,// 0x56
        0x7D35306D6D307B1B, 0x7D35306D6D317B1B, 0x7D35306D6D327B1B, 0x7D35306D6D337B1B, 0x7D35306D6D347B1B, 0x7D35306D6D357B1B, 0x7D35306D6D367B1B, 0x7D35306D6D377B1B,// 0x57
        0x7D697572307B1B, 0x7D697572317B1B, 0x7D697572327B1B, 0x7D697572337B1B, 0x7D697572347B1B, 0x7D697572357B1B, 0x7D697572367B1B, 0x7D697572377B1B,                // 0x58
        0x7D36306D6D307B1B, 0x7D36306D6D317B1B, 0x7D36306D6D327B1B, 0x7D36306D6D337B1B, 0x7D36306D6D347B1B, 0x7D36306D6D357B1B, 0x7D36306D6D367B1B, 0x7D36306D6D377B1B,// 0x59
        0x7D37306D6D307B1B, 0x7D37306D6D317B1B, 0x7D37306D6D327B1B, 0x7D37306D6D337B1B, 0x7D37306D6D347B1B, 0x7D37306D6D357B1B, 0x7D37306D6D367B1B, 0x7D37306D6D377B1B,// 0x5A
        0x7D707061307B1B, 0x7D707061317B1B, 0x7D707061327B1B, 0x7D707061337B1B, 0x7D707061347B1B, 0x7D707061357B1B, 0x7D707061367B1B, 0x7D707061377B1B,                // 0x5B
        0x7D38306D6D307B1B, 0x7D38306D6D317B1B, 0x7D38306D6D327B1B, 0x7D38306D6D337B1B, 0x7D38306D6D347B1B, 0x7D38306D6D357B1B, 0x7D38306D6D367B1B, 0x7D38306D6D377B1B,// 0x5C
        0x7D39306D6D307B1B, 0x7D39306D6D317B1B, 0x7D39306D6D327B1B, 0x7D39306D6D337B1B, 0x7D39306D6D347B1B, 0x7D39306D6D357B1B, 0x7D39306D6D367B1B, 0x7D39306D6D377B1B,// 0x5D
        0x7D41306D6D307B1B, 0x7D41306D6D317B1B, 0x7D41306D6D327B1B, 0x7D41306D6D337B1B, 0x7D41306D6D347B1B, 0x7D41306D6D357B1B, 0x7D41306D6D367B1B, 0x7D41306D6D377B1B,// 0x5E
        0x7D727770307B1B, 0x7D727770317B1B, 0x7D727770327B1B, 0x7D727770337B1B, 0x7D727770347B1B, 0x7D727770357B1B, 0x7D727770367B1B, 0x7D727770377B1B,                // 0x5F
        0x7D42306D6D307B1B, 0x7D42306D6D317B1B, 0x7D42306D6D327B1B, 0x7D42306D6D337B1B, 0x7D42306D6D347B1B, 0x7D42306D6D357B1B, 0x7D42306D6D367B1B, 0x7D42306D6D377B1B,// 0x60
        0x7D43306D6D307B1B, 0x7D43306D6D317B1B, 0x7D43306D6D327B1B, 0x7D43306D6D337B1B, 0x7D43306D6D347B1B, 0x7D43306D6D357B1B, 0x7D43306D6D367B1B, 0x7D43306D6D377B1B,// 0x61
        0x7D44306D6D307B1B, 0x7D44306D6D317B1B, 0x7D44306D6D327B1B, 0x7D44306D6D337B1B, 0x7D44306D6D347B1B, 0x7D44306D6D357B1B, 0x7D44306D6D367B1B, 0x7D44306D6D377B1B,// 0x62
        0x7D706C73307B1B, 0x7D706C73317B1B, 0x7D706C73327B1B, 0x7D706C73337B1B, 0x7D706C73347B1B, 0x7D706C73357B1B, 0x7D706C73367B1B, 0x7D706C73377B1B,                // 0x63
        0x7D45306D6D307B1B, 0x7D45306D6D317B1B, 0x7D45306D6D327B1B, 0x7D45306D6D337B1B, 0x7D45306D6D347B1B, 0x7D45306D6D357B1B, 0x7D45306D6D367B1B, 0x7D45306D6D377B1B,// 0x64
        0x7D46306D6D307B1B, 0x7D46306D6D317B1B, 0x7D46306D6D327B1B, 0x7D46306D6D337B1B, 0x7D46306D6D347B1B, 0x7D46306D6D357B1B, 0x7D46306D6D367B1B, 0x7D46306D6D377B1B,// 0x65
        0x2F, 0x7D2F317B1B, 0x7D2F327B1B, 0x7D2F337B1B, 0x7D2F347B1B, 0x7D2F357B1B, 0x7D2F367B1B, 0x7D2F377B1B,                                                        // 0x66
        0x7D30316D6D307B1B, 0x7D30316D6D317B1B, 0x7D30316D6D327B1B, 0x7D30316D6D337B1B, 0x7D30316D6D347B1B, 0x7D30316D6D357B1B, 0x7D30316D6D367B1B, 0x7D30316D6D377B1B,// 0x67
        0x7D31316D6D307B1B, 0x7D31316D6D317B1B, 0x7D31316D6D327B1B, 0x7D31316D6D337B1B, 0x7D31316D6D347B1B, 0x7D31316D6D357B1B, 0x7D31316D6D367B1B, 0x7D31316D6D377B1B,// 0x68
        0x0A, 0x7D6E5C317B1B, 0x7D6E5C327B1B, 0x7D6E5C337B1B, 0x7D6E5C347B1B, 0x7D6E5C357B1B, 0x7D6E5C367B1B, 0x7D6E5C377B1B,                                          // 0x69
        0x7D6B6177307B1B, 0x7D6B6177317B1B, 0x7D6B6177327B1B, 0x7D6B6177337B1B, 0x7D6B6177347B1B, 0x7D6B6177357B1B, 0x7D6B6177367B1B, 0x7D6B6177377B1B,                // 0x6A
        0x7D646E65307B1B, 0x7D646E65317B1B, 0x7D646E65327B1B, 0x7D646E65337B1B, 0x7D646E65347B1B, 0x7D646E65357B1B, 0x7D646E65367B1B, 0x7D646E65377B1B,                // 0x6B
        0x7D617363307B1B, 0x7D617363317B1B, 0x7D617363327B1B, 0x7D617363337B1B, 0x7D617363347B1B, 0x7D617363357B1B, 0x7D617363367B1B, 0x7D617363377B1B,                // 0x6C
        0x7D6D6F68307B1B, 0x7D6D6F68317B1B, 0x7D6D6F68327B1B, 0x7D6D6F68337B1B, 0x7D6D6F68347B1B, 0x7D6D6F68357B1B, 0x7D6D6F68367B1B, 0x7D6D6F68377B1B,                // 0x6D
        0x7D736E69307B1B, 0x7D736E69317B1B, 0x7D736E69327B1B, 0x7D736E69337B1B, 0x7D736E69347B1B, 0x7D736E69357B1B, 0x7D736E69367B1B, 0x7D736E69377B1B,                // 0x6E
        0x7D6C6564307B1B, 0x7D6C6564317B1B, 0x7D6C6564327B1B, 0x7D6C6564337B1B, 0x7D6C6564347B1B, 0x7D6C6564357B1B, 0x7D6C6564367B1B, 0x7D6C6564377B1B,                // 0x6F
        0x7D737363307B1B, 0x7D737363317B1B, 0x7D737363327B1B, 0x7D737363337B1B, 0x7D737363347B1B, 0x7D737363357B1B, 0x7D737363367B1B, 0x7D737363377B1B,                // 0x70
        0x7D647363307B1B, 0x7D647363317B1B, 0x7D647363327B1B, 0x7D647363337B1B, 0x7D647363347B1B, 0x7D647363357B1B, 0x7D647363367B1B, 0x7D647363377B1B,                // 0x71
        0x7D777363307B1B, 0x7D777363317B1B, 0x7D777363327B1B, 0x7D777363337B1B, 0x7D777363347B1B, 0x7D777363357B1B, 0x7D777363367B1B, 0x7D777363377B1B,                // 0x72
        0x7D646770307B1B, 0x7D646770317B1B, 0x7D646770327B1B, 0x7D646770337B1B, 0x7D646770347B1B, 0x7D646770357B1B, 0x7D646770367B1B, 0x7D646770377B1B,                // 0x73
        0x7D756770307B1B, 0x7D756770317B1B, 0x7D756770327B1B, 0x7D756770337B1B, 0x7D756770347B1B, 0x7D756770357B1B, 0x7D756770367B1B, 0x7D756770377B1B,                // 0x74
};

static union Data {

    Data() : raw{} {}

    struct Serial {
        ::Kernel::Serial s;
    } serial;
    struct Ps2 {
        uint8_t port;
        uint8_t scanCodeSet;
        uint8_t keyboardState;
        struct {
            bool hasE0 : 1;
            bool release : 1;
            bool overflow : 1;
            bool shift : 1;
            bool ctrl : 1;
            bool alt : 1;
            bool caps : 1;
        } flag;
        uint8_t enqueue;
        uint8_t dequeue;
        uint64_t data[128];
        uint64_t smallQueue;
        uint64_t map[512]{};
    } ps2;
    struct UsbKeyboard {
    } usbKeyboard;
    char raw[max(sizeof(::Kernel::Data::UsbKeyboard), max(sizeof(::Kernel::Data::Serial), sizeof(::Kernel::Data::Ps2)))];
} data;

void Input::init(InputMode mode) {
    usedMode = (uint8_t) mode;
    if (usedMode < size && initFunctions[usedMode])
        initFunctions[usedMode]();
}
uint8_t Input::readBlocking() {
    if (usedMode < size && readBFunctions[usedMode])
        return readBFunctions[usedMode]();
    while (true) {}
}
bool Input::isEmpty() {
    if (usedMode < size && isEmptyFunctions[usedMode])
        return isEmptyFunctions[usedMode]();
    return false;
}

void initSerial() {
    new (&data.serial.s)::Kernel::Serial(0x3F8);
    kout << "Input Serial at 0x3F8\n";
}
uint8_t readBlockingSerial() {
    while (isEmptySerial()) {}
    return data.serial.s.read();
}
bool isEmptySerial() {
    return data.serial.s.inputBufferEmpty();
}

static uint64_t ps2mapScanCode(uint16_t scanCode) {
    switch (scanCode) {
        case 0x058:
            data.ps2.flag.caps = !data.ps2.flag.caps;
            break;
        case 0x012:
        case 0x059:
            data.ps2.flag.shift = true;
            break;
        case 0x212:
        case 0x259:
            data.ps2.flag.shift = false;
            break;
        case 0x011:
        case 0x111:
            data.ps2.flag.alt = true;
            break;
        case 0x211:
        case 0x311:
            data.ps2.flag.alt = false;
            break;
        case 0x014:
        case 0x114:
            data.ps2.flag.ctrl = true;
            break;
        case 0x214:
        case 0x314:
            data.ps2.flag.ctrl = false;
            break;
        default: {
            if (scanCode & 0x200) {
                return 0;// release key
            }
            uint64_t keyCode = data.ps2.map[scanCode];
            if (keyCode == 0) {
                return ~0llu;
            } else {
                keyCode <<= 3;
                if (data.ps2.flag.shift) keyCode |= 0b1 << 0;
                if (data.ps2.flag.caps) keyCode ^= 0b1 << 0;
                if (data.ps2.flag.ctrl) keyCode |= 0b1 << 1;
                if (data.ps2.flag.alt) keyCode |= 0b1 << 2;
                uint64_t result = keyLayout[keyCode];
                if (result == 0) {
                    kout << "KeyLayout incomplete! 0x" << Hex(keyCode >> 3) << '\n';
                    return 0;
                } else {
                    return result;
                }
            }
        }
    }
    return 0;
}

static void ps2Handler(uint8_t ch) {
    if (ch == 0xFA) {
        return;
    }
    switch (data.ps2.keyboardState) {
        case 0:
            data.ps2.scanCodeSet = ch;
            data.ps2.keyboardState++;
            break;
        case 1:
            if (data.ps2.scanCodeSet != 2) {
                kout << "Only scanocde set 2 is supported!\n";
                return;
            }
            if (ch == 0xE0) {
                data.ps2.flag.hasE0 = true;
            } else if (ch == 0xF0) {
                data.ps2.flag.release = true;
            } else {
                uint16_t code = 0;
                code |= data.ps2.flag.hasE0 ? 0b1 << 8 : 0;
                code |= data.ps2.flag.release ? 0b1 << 9 : 0;
                code |= ch;
                data.ps2.flag.hasE0 = false;
                data.ps2.flag.release = false;
                uint8_t nextEnqueue = (data.ps2.enqueue + 1) % 128;
                uint64_t result = ps2mapScanCode(code);
                if (result == 0) {
                    break;
                }
                if (result == ~0llu) {
                    kout << "Unknown scancode: " << Hex(code) << '\n';
                }
                if (nextEnqueue == data.ps2.dequeue) {
                    data.ps2.flag.overflow = true;
                } else {
                    data.ps2.data[data.ps2.enqueue] = result;
                    data.ps2.enqueue = nextEnqueue;
                }
            }
        default:
            break;
    }
}

void initPs2() {
    data.ps2.map[0x01] = 0x01; //'F9'
    data.ps2.map[0x03] = 0x02; //'F5'
    data.ps2.map[0x04] = 0x03; //'F3'
    data.ps2.map[0x05] = 0x04; //'F1'
    data.ps2.map[0x06] = 0x05; //'F2'
    data.ps2.map[0x07] = 0x06; //'F12'
    data.ps2.map[0x09] = 0x07; //'F10'
    data.ps2.map[0x0A] = 0x08; //'F8'
    data.ps2.map[0x0B] = 0x09; //'F6'
    data.ps2.map[0x0C] = 0x0A; //'F4'
    data.ps2.map[0x0D] = 0x0B; //'\t'
    data.ps2.map[0x0E] = 0x0C; //'`'
    data.ps2.map[0x15] = 0x0D; //'Q'
    data.ps2.map[0x16] = 0x0E; //'1'
    data.ps2.map[0x1A] = 0x0F; //'Z'
    data.ps2.map[0x1B] = 0x10; //'S'
    data.ps2.map[0x1C] = 0x11; //'A'
    data.ps2.map[0x1D] = 0x12; //'W'
    data.ps2.map[0x1E] = 0x13; //'2'
    data.ps2.map[0x21] = 0x14; //'C'
    data.ps2.map[0x22] = 0x15; //'X'
    data.ps2.map[0x23] = 0x16; //'D'
    data.ps2.map[0x24] = 0x17; //'E'
    data.ps2.map[0x25] = 0x18; //'4'
    data.ps2.map[0x26] = 0x19; //'3'
    data.ps2.map[0x29] = 0x1A; //' '
    data.ps2.map[0x2A] = 0x1B; //'V'
    data.ps2.map[0x2B] = 0x1C; //'F'
    data.ps2.map[0x2C] = 0x1D; //'T'
    data.ps2.map[0x2D] = 0x1E; //'R'
    data.ps2.map[0x2E] = 0x1F; //'5'
    data.ps2.map[0x31] = 0x20; //'N'
    data.ps2.map[0x32] = 0x21; //'B'
    data.ps2.map[0x33] = 0x22; //'H'
    data.ps2.map[0x34] = 0x23; //'G'
    data.ps2.map[0x35] = 0x24; //'Y'
    data.ps2.map[0x36] = 0x25; //'6'
    data.ps2.map[0x3A] = 0x26; //'M'
    data.ps2.map[0x3B] = 0x27; //'J'
    data.ps2.map[0x3C] = 0x28; //'U'
    data.ps2.map[0x3D] = 0x29; //'7'
    data.ps2.map[0x3E] = 0x2A; //'8'
    data.ps2.map[0x41] = 0x2B; //','
    data.ps2.map[0x42] = 0x2C; //'K'
    data.ps2.map[0x43] = 0x2D; //'I'
    data.ps2.map[0x44] = 0x2E; //'O'
    data.ps2.map[0x45] = 0x2F; //'0'
    data.ps2.map[0x46] = 0x30; //'9'
    data.ps2.map[0x49] = 0x31; //'.'
    data.ps2.map[0x4A] = 0x32; //'/'
    data.ps2.map[0x4B] = 0x33; //'L'
    data.ps2.map[0x4C] = 0x34; //';'
    data.ps2.map[0x4D] = 0x35; //'P'
    data.ps2.map[0x4E] = 0x36; //'-'
    data.ps2.map[0x52] = 0x37; //'''ä
    data.ps2.map[0x54] = 0x38; //'['ü
    data.ps2.map[0x55] = 0x39; //'='´
    data.ps2.map[0x5A] = 0x3A; //'enter'
    data.ps2.map[0x5B] = 0x3B; //']'
    data.ps2.map[0x5D] = 0x3C; //'\'
    data.ps2.map[0x66] = 0x3D; //8
    data.ps2.map[0x69] = 0x3E; //'1'
    data.ps2.map[0x6B] = 0x3F; //'4'
    data.ps2.map[0x6C] = 0x40; //'7'
    data.ps2.map[0x70] = 0x41; //'0'
    data.ps2.map[0x71] = 0x42; //'.'
    data.ps2.map[0x72] = 0x43; //'2'
    data.ps2.map[0x73] = 0x44; //'5'
    data.ps2.map[0x74] = 0x45; //'6'
    data.ps2.map[0x75] = 0x46; //'8'
    data.ps2.map[0x76] = 0x47; //'escape'
    data.ps2.map[0x77] = 0x48; //'NumberLock'
    data.ps2.map[0x78] = 0x49; //'F11'
    data.ps2.map[0x79] = 0x4A; //'+'
    data.ps2.map[0x7A] = 0x4B; //'3'
    data.ps2.map[0x7B] = 0x4C; //'-'
    data.ps2.map[0x7C] = 0x4D; //'*'
    data.ps2.map[0x7D] = 0x4E; //'9'
    data.ps2.map[0x7E] = 0x4F; //'ScrollLock'
    data.ps2.map[0x83] = 0x50; //'F7'
    data.ps2.map[0x110] = 0x51;//'(multimedia) WWW search'
    data.ps2.map[0x115] = 0x52;//'(multimedia) previous track'
    data.ps2.map[0x118] = 0x53;//'(multimedia) WWW favourites'
    data.ps2.map[0x11F] = 0x54;//'left GUI'
    data.ps2.map[0x120] = 0x55;//'(multimedia) WWW refresh'
    data.ps2.map[0x121] = 0x56;//'(multimedia) volume down'
    data.ps2.map[0x123] = 0x57;//'(multimedia) mute'
    data.ps2.map[0x127] = 0x58;//'right GUI'
    data.ps2.map[0x128] = 0x59;//'(multimedia) WWW stop'
    data.ps2.map[0x12B] = 0x5A;//'(multimedia) calculator'
    data.ps2.map[0x12F] = 0x5B;//'apps'
    data.ps2.map[0x130] = 0x5C;//'(multimedia) WWW forward'
    data.ps2.map[0x132] = 0x5D;//'(multimedia) volume up'
    data.ps2.map[0x134] = 0x5E;//'(multimedia) play/pause'
    data.ps2.map[0x137] = 0x5F;//'(ACPI) power'
    data.ps2.map[0x138] = 0x60;//'(multimedia) WWW back'
    data.ps2.map[0x13A] = 0x61;//'(multimedia) WWW home'
    data.ps2.map[0x13B] = 0x62;//'(multimedia) stop'
    data.ps2.map[0x13F] = 0x63;//'(ACPI) sleep'
    data.ps2.map[0x140] = 0x64;//'(multimedia) my computer'
    data.ps2.map[0x148] = 0x65;//'(multimedia) email'
    data.ps2.map[0x14A] = 0x66;//'/'
    data.ps2.map[0x14D] = 0x67;//'(multimedia) next track'
    data.ps2.map[0x150] = 0x68;//'(multimedia) media select'
    data.ps2.map[0x15A] = 0x69;//'\n'
    data.ps2.map[0x15E] = 0x6A;//'(ACPI) wake'
    data.ps2.map[0x169] = 0x6B;//'end'
    data.ps2.map[0x16B] = 0x6C;//'cursor left'
    data.ps2.map[0x16C] = 0x6D;//'home'
    data.ps2.map[0x170] = 0x6E;//'insert'
    data.ps2.map[0x171] = 0x6F;//'delete'
    data.ps2.map[0x172] = 0x70;//'cursor down'
    data.ps2.map[0x174] = 0x71;//'cursor right'
    data.ps2.map[0x175] = 0x72;//'cursor up'
    data.ps2.map[0x17A] = 0x73;//'page down'
    data.ps2.map[0x17D] = 0x74;//'page up'

    if (uint8_t ps2 = PS2::getPorts(); ps2) {
        for (uint8_t i = 0; i < 2; ++i) {
            if (ps2 & (0b1 << 1)) {
                if (PS2::getType(i) == 0xAB83) {
                    data.ps2.port = i;
                    break;
                }
            }
        }
    }
    kout << "Input PS/2 Port " << (uint64_t) (data.ps2.port + 1) << '\n';
    PS2::setInterruptOverride(data.ps2.port, ps2Handler);
    PS2::write(data.ps2.port, 0xF0);
    PS2::write(data.ps2.port, 0x00);
}
uint8_t readBlockingPs2() {
    if (data.ps2.smallQueue) {
        uint8_t ch = data.ps2.smallQueue & 0xFF;
        data.ps2.smallQueue >>= 8;
        return ch;
    }
    while (isEmptyPs2()) {}
    uint64_t result = data.ps2.data[data.ps2.dequeue];
    data.ps2.dequeue = (data.ps2.dequeue + 1) % 128;
    data.ps2.smallQueue = result;
    uint8_t ch = data.ps2.smallQueue & 0xFF;
    data.ps2.smallQueue >>= 8;
    return ch;
}
bool isEmptyPs2() {
    return data.ps2.enqueue == data.ps2.dequeue && data.ps2.smallQueue == 0;
}

void initUsbKeyboard() {}
uint8_t readBlockingUsbKeyboard() {
    while (true) {}
}
bool isEmptyUsbKeyboard() {
    return false;
}

}// namespace Kernel