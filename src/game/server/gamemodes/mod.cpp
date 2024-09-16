/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include "mod.h"
#include "../gamecontext.h"
#include "../gamecontroller.h"
#include "string"

CGameControllerMOD::CGameControllerMOD(class CGameContext *pGameServer)
: IGameController(pGameServer)
{
	// Exchange this to a string that identifies your game mode.
	// DM, TDM and CTF are reserved for teeworlds original modes.
	m_pGameType = "gGame";
	//m_GameFlags = GAMEFLAG_TEAMS; // GAMEFLAG_TEAMS makes it a two-team gamemode
}

void CGameControllerMOD::UpdateWeapons(CPlayer* pPlayer) {
	if (!pPlayer) { return;}

	pPlayer->GetCharacter()->RemoveWeapons();
	int score = pPlayer->m_Score;
	int weaponOrder[] = {
		WEAPON_GUN,
		WEAPON_SHOTGUN,
		WEAPON_GRENADE,
		WEAPON_RIFLE,
		WEAPON_HAMMER,
	};

	int weaponAmmo[] = {
		-1,
		5,
		5,
		5,
		-1
	};
	int weapon = 0;
	int ammo = 0;
	if (score / 2 < sizeof(weaponOrder) / 4) {
		weapon = weaponOrder[score / 2];
		ammo = weaponAmmo[score / 2];
	}
	else {
		GameServer()->SendChat(-1, CGameContext::CHAT_ALL, "GG");
		GameServer()->SendBroadcast((Server()->ClientName(pPlayer->GetCID()) + std::string(" won!")).c_str(), -1);
		return;
	}
	pPlayer->GetCharacter()->GiveWeapon(weapon, ammo);
	pPlayer->GetCharacter()->SetWeapon(weapon);
}

void CGameControllerMOD::Tick()
{
	// this is the main part of the gamemode, this function is run every tick

	IGameController::Tick();
}

void CGameControllerMOD::OnCharacterSpawn(class CCharacter* pChr) {
	pChr->IncreaseHealth(10);

	UpdateWeapons(pChr->GetPlayer());
	GameServer()->SendBroadcast("test", -1);
}

int CGameControllerMOD::OnCharacterDeath(class CCharacter* pVictim, class CPlayer* pKiller, int Weapon)
{
	int mode = IGameController::OnCharacterDeath(pVictim, pKiller, Weapon);
	UpdateWeapons(pKiller);
	return mode;
}