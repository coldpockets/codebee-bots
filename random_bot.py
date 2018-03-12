from helper_package_python.constants import ActionType

from helper_package_python.bot import Bot
from helper_package_python.action import Action

from random import randint

class RandomBot(Bot):
	def __init__(self):
		super().__init__("RandomBot")

	def getActions(self):
		actions = []

		for beeCell in self.beeCells:
			move = randint(0, 4)

			actions.append(Action(
				ActionType.MOVE,
				beeCell.pos,
				move,
				(move - 1) if (move > 0) else beeCell.bee.face
			))

		return actions

def main():
	randomBot = RandomBot()
	randomBot.run()

main()
