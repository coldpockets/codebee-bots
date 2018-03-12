from helper_package_python.constants import ActionType, BEE_POLLEN_AMOUNT

from helper_package_python.bot import Bot
from helper_package_python.action import Action

class HarvesterBot2(Bot):
	def __init__(self):
		super().__init__("HarvesterBot2")

	def getActions(self):
		actions = []

		for beeCell in self.beeCells:
			bee = beeCell.bee
			pos = beeCell.pos

			# If the bees have pollen, move to queen.
			if bee.pollen >= bee.count:
				if !self.isBesideHiveOrQueen(pos):
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

		if not self.hiveCells:
			actions.append(Action(
				ActionType.CREATE_HIVE
			))
		else:
			actions.append(Action(
				ActionType.SPAWN,
				self.hiveCells[0].pos,
				self.queenBee.pollen / BEE_POLLEN_AMOUNT
			))

		return actions

def main():
	harvesterBot2 = HarvesterBot2()
	harvesterBot2.run()

main()