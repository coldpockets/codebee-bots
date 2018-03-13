from helper_package_python.constants import ActionType, Face

from helper_package_python.bot import Bot
from helper_package_python.action import Action

class HarvesterBot(Bot):
	def __init__(self):
		super().__init__("HarvesterBot")

	def init(self):
		pass

	def getActions(self):
		actions = []

		for beeCell in self.beeCells:
			bee = beeCell.bee
			pos = beeCell.pos

			# If the bees have pollen, move to queen.
			if bee.pollen >= bee.count:
				if not self.isBesideHiveOrQueen(pos):
					path = self.getMinPath(pos, self.queenBee.pos)

					actions.append(Action(
						ActionType.MOVE,
						pos,
						path.move,
						Face(path.move - 1) # Face move direction
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

		return actions

def main():
	harvesterBot = HarvesterBot()
	harvesterBot.run()

main()
