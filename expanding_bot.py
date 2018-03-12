from helper_package_python.constants import (
	ActionType, 
	Move, 
	Face, 
	Flower, 
	HIVE_POLLEN_AMOUNT, 
	BEE_POLLEN_AMOUNT,
	MAX_BEES,
	NEUTRAL_ID
)

from helper_package_python.bot import Bot
from helper_package_python.action import Action

from random import randint
import math

BEES_TO_HIVE_RATIO = 30

class ExpandingBot(Bot):
	def __init__(self):
		super().__init__("ExpandingBot")

	def getActions(self):
		actions = []

		for beeCell in self.beeCells:
			bee = beeCell.bee
			pos = beeCell.pos

			if bee.pollen >= bee.count:
				# If the bees have pollen, move to queen.
				if not self.isBesideHiveOrQueen(pos):
					minPath = self.getMinPath(pos, self.queenBee.pos)

					for hiveCell in self.hiveCells:
						path = self.getMinPath(pos, hiveCell.pos)
						if path.distance < minPath.distance:
							minPath = path

					actions.append(Action(
						ActionType.MOVE,
						pos,
						minPath.move,
						Face(minPath.move - 1) # Face move direction
					))
			else:
				# Move to closest flower to collect pollen.
				firstFlower = self.flowerCells[0]
				minPath = self.getMinPath(pos, firstFlower.pos)
				
				for i in range(1, len(self.flowerCells)):
					path = self.getMinPath(pos, self.flowerCells[i].pos)
					if path.distance < minPath.distance:
						minPath = path

				actions.append(Action(
					ActionType.MOVE,
					pos,
					minPath.move,
					Face(minPath.move - 1) # Face move direction
				))

		queenPos = self.queenBee.pos
		pollenNotUsed = self.queenBee.pollen
		posPotency = self.currMap.map[queenPos.y][queenPos.x].potency

		# Create hive if the bees to hive ratio is higer than what we want
		if (self.totalBees > BEES_TO_HIVE_RATIO * len(self.hiveCells)) and (posPotency == Flower.NONE):
			pollenNotUsed -= HIVE_POLLEN_AMOUNT
			actions.append(Action(
				ActionType.CREATE_HIVE
			))

		# If queen is on hive or flower, move to an open spot.
		if not self.isEmpty(queenPos):
			if self.isEmpty(self.getBoundedPos(queenPos.x + 1, queenPos.y)):
				actions.append(Action(
					ActionType.MOVE_QUEEN,
					None,
					Move.RIGHT,
					Face.RIGHT
				))
			elif self.isEmpty(self.getBoundedPos(queenPos.x, queenPos.y + 1)):
				actions.append(Action(
					ActionType.MOVE_QUEEN,
					None,
					Move.DOWN,
					Face.DOWN
				))
			elif self.isEmpty(self.getBoundedPos(queenPos.x - 1, queenPos.y)):
				actions.append(Action(
					ActionType.MOVE_QUEEN,
					None,
					Move.LEFT,
					Face.LEFT
				))
			elif self.isEmpty(self.getBoundedPos(queenPos.x, queenPos.y - 1)):
				actions.append(Action(
					ActionType.MOVE_QUEEN,
					None,
					Move.UP,
					Face.UP
				))
			else:
				# Move randomly
				move = randint(1, 4)
				actions.append(Action(
					ActionType.MOVE_QUEEN,
					None,
					Move(move),
					Face(move - 1) # Faces move direction
				))

		if not self.hiveCells:
			totalBeesPerHive = math.floor(math.floor(pollenNotUsed / BEE_POLLEN_AMOUNT) / len(self.hiveCells))
			for hiveCell in self.hiveCells:
				# Makes sure we dont overspawn bees
				if totalBeesPerHive > MAX_BEES:
					actions.append(Action(
						ActionType.SPAWN,
						hiveCell.pos,
						MAX_BEES
					))
				else:
					actions.append(Action(
						ActionType.SPAWN,
						hiveCell.pos,
						totalBeesPerHive
					))

		return actions

	def isEmpty(self, pos):
		return self.currMap.map[pos.y][pos.x].potency == Flower.NONE and \
            self.currMap.map[pos.y][pos.x].ownerId == NEUTRAL_ID

def main():
	expandingBot = ExpandingBot()
	expandingBot.run()

main()