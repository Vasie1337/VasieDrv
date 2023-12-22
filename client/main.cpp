#include "driver.h"

#define UWORLD 0x458CB10
#define M_PI       3.14159265358979323846   // pi

struct FVector
{
public:
    float                                        X;                                                 // 0x0(0x4)(Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
    float                                        Y;                                                 // 0x4(0x4)(Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
    float                                        Z;                                                 // 0x8(0x4)(Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)

    inline FVector()
        : X(0.0), Y(0.0), Z(0.0)
    {
    }

    inline FVector(decltype(X) Value)
        : X(Value), Y(Value), Z(Value)
    {
    }

    inline FVector(decltype(X) x, decltype(Y) y, decltype(Z) z)
        : X(x), Y(y), Z(z)
    {
    }

    inline bool operator==(const FVector& Other) const
    {
        return X == Other.X && Y == Other.Y && Z == Other.Z;
    }

    inline bool operator!=(const FVector& Other) const
    {
        return X != Other.X || Y != Other.Y || Z != Other.Z;
    }

    inline float Distance(const FVector& Other) const
    {
        float DeltaX = X - Other.X;
        float DeltaY = Y - Other.Y;
        float DeltaZ = Z - Other.Z;

        return std::sqrt(DeltaX * DeltaX + DeltaY * DeltaY + DeltaZ * DeltaZ);
    }

    FVector operator+(const FVector& Other) const
    {
        return { X + Other.X, Y + Other.Y, Z + Other.Z };
    }

    FVector operator-(const FVector& Other) const
    {
        return { X - Other.X, Y - Other.Y, Z - Other.Z };
    }

    FVector operator*(decltype(X) Scalar) const
    {
        return { X * Scalar, Y * Scalar, Z * Scalar };
    }

    FVector operator/(decltype(X) Scalar) const
    {
        if (Scalar == 0.0f)
            return FVector();

        return { X / Scalar, Y / Scalar, Z / Scalar };
    }
};

struct FRotator
{
public:
    float                                        Pitch;                                             // 0x0(0x4)(Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
    float                                        Yaw;                                               // 0x4(0x4)(Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
    float                                        Roll;                                              // 0x8(0x4)(Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)

    inline FRotator()
        : Pitch(0.0), Yaw(0.0), Roll(0.0)
    {
    }

    inline FRotator(decltype(Pitch) Value)
        : Pitch(Value), Yaw(Value), Roll(Value)
    {
    }

    inline FRotator(decltype(Pitch) pitch, decltype(Yaw) yaw, decltype(Roll) roll)
        : Pitch(pitch), Yaw(yaw), Roll(roll)
    {
    }


    inline bool operator==(const FRotator& Other) const
    {
        return Pitch == Other.Pitch && Yaw == Other.Yaw && Roll == Other.Roll;
    }

    inline bool operator!=(const FRotator& Other) const
    {
        return Pitch != Other.Pitch || Yaw != Other.Yaw || Roll != Other.Roll;
    }

    FRotator operator+(const FRotator& Other) const;

    FRotator operator-(const FRotator& Other) const;

    FRotator operator*(decltype(Pitch) Scalar) const;

    FRotator operator/(decltype(Pitch) Scalar) const;
};

struct camera {
    struct FVector Location;
    struct FRotator Rotation;
    float FOV;
};

class cache
{
public:
    inline static uintptr_t Uworld;
    inline static uintptr_t PersistentLevel;
    inline static uintptr_t GameState;
    inline static uintptr_t GameInstance;
    inline static uintptr_t LocalPlayers;
    inline static uintptr_t LocalPlayer;
    inline static uintptr_t PlayerController;
    inline static uintptr_t Character;
    inline static uintptr_t AcknowledgedPawn;
    inline static uintptr_t PlayerState;
    inline static uintptr_t Mesh;
    inline static uintptr_t RootComponent;
    inline static uintptr_t PlayerCameraManager;
    inline static uintptr_t CharacterMovement;
    inline static uintptr_t PrimaryWeapon;
    inline static FVector RelativeLocation;

    inline static uintptr_t PlayerArray;
    inline static int PlayerCount;

    inline static camera LocalCamera;
};

static camera getCamera()
{
    return c_driver::readMem<camera>(cache::PlayerCameraManager + 0xE70 + 0x10);
}

class c_cheat {
public:
    static void exploits()
    {
        
    }
};

int main()
{
	c_driver::initComm(L"FPSChess-Win64-Shipping.exe"); // example
	c_driver::getBase();

	printf("base: 0x%p\n", (void*)c_driver::targetBase);

    cache::Uworld = c_driver::readMem<uintptr_t>(c_driver::targetBase + 0x4B80F70);
    if (!cache::Uworld) return 1;
    printf("Uworld: %p\n", cache::Uworld);

    cache::PersistentLevel = c_driver::readMem<uintptr_t>(cache::Uworld + 0x30);
    if (!cache::PersistentLevel) return 1;
    printf("PersistentLevel: %p\n", cache::PersistentLevel);

    cache::GameState = c_driver::readMem<uintptr_t>(cache::Uworld + 0x120);
    if (!cache::GameState) return 1;
    printf("GameState: %p\n", cache::GameState);

    cache::GameInstance = c_driver::readMem<uintptr_t>(cache::Uworld + 0x180);
    if (!cache::GameInstance) return 1;
    printf("GameInstance: %p\n", cache::GameInstance);

    cache::LocalPlayers = c_driver::readMem<uintptr_t>(cache::GameInstance + 0x38);
    if (!cache::LocalPlayers) return 1;

    cache::LocalPlayer = c_driver::readMem<uintptr_t>(cache::LocalPlayers);
    if (!cache::LocalPlayer) return 1;
    printf("LocalPlayer: %p\n", cache::LocalPlayer);

    cache::PlayerController = c_driver::readMem<uintptr_t>(cache::LocalPlayer + 0x30);
    if (!cache::PlayerController) return 1;

    cache::Character = c_driver::readMem<uintptr_t>(cache::PlayerController + 0x260);
    if (!cache::Character) return 1;

    //cache::PrimaryWeapon = c_driver::readMem<uintptr_t>(cache::Character + 0x558);
    //if (!cache::PrimaryWeapon) return 1;

    cache::PlayerCameraManager = c_driver::readMem<uintptr_t>(cache::PlayerController + 0x2B8);
    if (!cache::PlayerCameraManager) return 1;

    cache::LocalCamera = getCamera();
    if (!cache::LocalCamera.FOV) return 1;

    cache::CharacterMovement = c_driver::readMem<uintptr_t>(cache::Character + 0x288);
    if (!cache::CharacterMovement) return 1;

    cache::Mesh = c_driver::readMem<uintptr_t>(cache::Character + 0x280);
    if (!cache::Mesh) return 1;

    cache::AcknowledgedPawn = c_driver::readMem<uintptr_t>(cache::PlayerController + 0x2A0);
    if (!cache::AcknowledgedPawn) return 1;
    printf("AcknowledgedPawn: %p\n", cache::AcknowledgedPawn);

    cache::RootComponent = c_driver::readMem<uintptr_t>(cache::AcknowledgedPawn + 0x130);
    if (!cache::RootComponent) return 1;

    cache::PlayerState = c_driver::readMem<uintptr_t>(cache::AcknowledgedPawn + 0x240);
    if (!cache::PlayerState) return 1;

    INT32 LocalTeamID = c_driver::readMem<INT32>(cache::PlayerState + 0x360);

    cache::PlayerArray = c_driver::readMem<uintptr_t>(cache::GameState + 0x238);
    if (!cache::PlayerArray) return 1;

    cache::PlayerCount = c_driver::readMem<int>(cache::GameState + 0x238 + sizeof(uintptr_t));
    if (!cache::PlayerCount) return 1;

    cache::RelativeLocation = c_driver::readMem<FVector>(cache::RootComponent + 0x11C);

    float ClosestDistance = FLT_MAX;
    FVector ClosestLocation;

    for (int i = 0; i < cache::PlayerCount; i++)
    {
        uintptr_t PlayerState = c_driver::readMem<uintptr_t>(cache::PlayerArray + (i * sizeof(uintptr_t)));
        if (!PlayerState) return 1;

        INT32 TeamID = c_driver::readMem<INT32>(PlayerState + 0x360);
        if (LocalTeamID == TeamID) return 1;

        uintptr_t PawnPrivate = c_driver::readMem<uintptr_t>(PlayerState + 0x280);
        if (!PawnPrivate) return 1;

        uintptr_t RootComponent = c_driver::readMem<uintptr_t>(PawnPrivate + 0x130);
        if (!RootComponent) return 1;

        FVector RelativeLocation = c_driver::readMem<FVector>(RootComponent + 0x11C);
        RelativeLocation.Z -= 30.f;
        float Distance = cache::RelativeLocation.Distance(RelativeLocation);

        if (Distance < ClosestDistance)
        {
            ClosestDistance = Distance;
            ClosestLocation = RelativeLocation;
        }
    }

    //float ViewPitchMin = -89.9f;
    //float ViewPitchMax = 89.9f;
    //float ViewYawMin = 0.000f;
    //float ViewYawMax = 359.999f;
    //
    //c_driver::writeMem<float>(cache::PlayerCameraManager + 0x27E4, ViewPitchMin);
    //c_driver::writeMem<float>(cache::PlayerCameraManager + 0x27E8, ViewPitchMax);
    //c_driver::writeMem<float>(cache::PlayerCameraManager + 0x27EC, ViewYawMin);
    //c_driver::writeMem<float>(cache::PlayerCameraManager + 0x27F0, ViewYawMax);
    //
    //FVector VectorPos = ClosestLocation - cache::RelativeLocation;
    //
    //float distance = sqrtf(VectorPos.X * VectorPos.X + VectorPos.Y * VectorPos.Y + VectorPos.Z * VectorPos.Z);
    //float x, y;
    //x = -((acosf(VectorPos.Z / distance) * (float)(180.0f / M_PI)) - 90.f);
    //y = atan2f(VectorPos.Y, VectorPos.X) * (float)(180.0f / M_PI);
    //
    //if (GetAsyncKeyState(VK_RBUTTON) && ClosestDistance < FLT_MAX)
    //{
    //    c_driver::writeMem<float>(cache::PlayerCameraManager + 0x27E4, x);
    //    c_driver::writeMem<float>(cache::PlayerCameraManager + 0x27E8, x);
    //    c_driver::writeMem<float>(cache::PlayerCameraManager + 0x27EC, y);
    //    c_driver::writeMem<float>(cache::PlayerCameraManager + 0x27F0, y);
    //}

    c_cheat::exploits();

}