//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CPlayerManager.cpp
// Project: Server
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

CPlayerManager::CPlayerManager()
{
	for(EntityId playerId = 0; playerId < PLAYER_MAX; playerId++)
		m_pPlayers[playerId] = NULL;
}

CPlayerManager::~CPlayerManager()
{
	// Loop through all players
	for(EntityId i = 0; i < PLAYER_MAX; i++)
	{
		// Is the current player active?
		if(IsActive(i))
		{
			// Delete the current player
			Delete(i);
		}
	}
}

bool CPlayerManager::Add(EntityId playerId, String strName)
{
	// Is this player already added?
	if(IsActive(playerId))
		return false;

	// Create the player instance
	m_pPlayers[playerId] = new CPlayer(playerId, strName);

	// Was the player instance created?
	if(m_pPlayers[playerId])
	{
		// Add the player for everyone
		m_pPlayers[playerId]->AddForWorld();
	}

	return true;
}

bool CPlayerManager::Delete(EntityId playerId)
{
	// Is this player not active?
	if(!IsActive(playerId))
		return false;

	// Delete the player for everyone
	m_pPlayers[playerId]->DeleteForWorld();

	// Delete the player instance
	delete m_pPlayers[playerId];
	m_pPlayers[playerId] = NULL;
	return true;
}

bool CPlayerManager::IsActive(EntityId playerId)
{
	// Is the player id more than PLAYER_MAX?
	if(playerId > PLAYER_MAX)
		return false;

	return (m_pPlayers[playerId] != NULL);
}

CPlayer * CPlayerManager::Get(EntityId playerId)
{
	// Is this player not active?
	if(!IsActive(playerId))
		return NULL;

	return m_pPlayers[playerId];
}

EntityId CPlayerManager::GetCount()
{
	EntityId count = 0;

	// Loop through all players
	for(EntityId i = 0; i < PLAYER_MAX; i++)
	{
		// Is the current player active?
		if(IsActive(i))
		{
			// Increment the player count
			count++;
		}
	}

	return count;
}

void CPlayerManager::HandlePlayerJoin(EntityId playerId)
{
	// Loop through all players
	for(EntityId i = 0; i < PLAYER_MAX; i++)
	{
		// Is the current player not this player and active?
		if(i != playerId && IsActive(i))
		{
			// Add the current player for this player
			m_pPlayers[i]->AddForPlayer(playerId);

			// Is the current player spawned?
			if(m_pPlayers[i]->IsSpawned())
			{
				// Spawn the current player for this player
				m_pPlayers[i]->SpawnForPlayer(playerId);
			}
		}
	}
}
