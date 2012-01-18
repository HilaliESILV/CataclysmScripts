/*
 * Copyright (C) 2011 - 2012 ArkCORE <http://www.arkania.net/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "throne_of_the_four_winds.h"

enum SlipstreamEnums
{
	// Land Positions
	DIR_WEST_TO_SOUTH,
	DIR_SOUTH_TO_WEST,
	DIR_NORTH_TO_WEST,
	DIR_WEST_TO_NORTH,
	DIR_EAST_TO_NORTH,
	DIR_NORTH_TO_EAST,
	DIR_SOUTH_TO_EAST,
	DIR_EAST_TO_SOUTH,
	DIR_ERROR,

	// Spells
	SPELL_SLIPSTREAM_BUFF = 87740,
	SPELL_SLIPSTREAM_PLAYER_VISUAL = 85063,
};

Position const SlipstreamPositions[8] =
{
	{-245.141129f,861.474060f,197.386398f, 0},
	{-92.116440f,1010.796448f,197.475754f, 0},
	{-5.322055f,1010.573608f,197.520096f, 0},
	{144.480469f, 857.187927f, 197.594208f, 0},
	{144.221481f,770.720154f,197.629150f, 0},
	{-9.268366f,620.736328f,197.567032f, 0},
	{-96.089645f, 621.198730f,197.499115f, 0},
	{-245.653870f,774.446472f,197.507156f, 0},
};

class npc_slipstream_raid : public CreatureScript
{
public:
    npc_slipstream_raid() : CreatureScript("npc_slipstream_raid") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_slipstream_raidAI (creature);
    }

    struct npc_slipstream_raidAI : public ScriptedAI
    {
        npc_slipstream_raidAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();

			SlipstreamPosition = 8;

			for(uint8 i = 0; i<=7; i++)
				if(me->GetDistance2d(SlipstreamPositions[i].GetPositionX(), SlipstreamPositions[i].GetPositionY()) < 6)
					SlipstreamPosition = i;

			if(SlipstreamPosition == DIR_ERROR)
				me->DespawnOrUnsummon();

			SlipstreamPosition += (SlipstreamPosition == DIR_WEST_TO_SOUTH || SlipstreamPosition == DIR_NORTH_TO_WEST ||
				SlipstreamPosition == DIR_EAST_TO_NORTH || SlipstreamPosition == DIR_SOUTH_TO_EAST ) ? 1 : -1;
        }

        InstanceScript* instance;
		uint8 SlipstreamPosition;

        void UpdateAI(const uint32 diff)
		{
		}

		void MoveInLineOfSight(Unit* who)
        {
			if((!who->HasAura(SPELL_SLIPSTREAM_BUFF)) && who->GetDistance(me)<4)
			{
				me->CastSpell(who,SPELL_SLIPSTREAM_BUFF, true);
				who->CastSpell(who,SPELL_SLIPSTREAM_PLAYER_VISUAL, true);
				who->GetMotionMaster()->MoveJump(SlipstreamPositions[SlipstreamPosition].GetPositionX(),SlipstreamPositions[SlipstreamPosition].GetPositionY(),198.458481f,1,6);

				//who->GetMotionMaster()->MoveJump(-256.062378f,850.749207f,198.457733f,1,5);
			}
        }
    };
};

void AddSC_throne_of_the_four_winds()
{
    new npc_slipstream_raid();
}