from helper_package_python.constants import NEUTRAL_ID

class Cell:
	def __init__(self, pos, potency):
		self.pos = pos
		self.potency = potency
		self.ownerId = NEUTRAL_ID
		self.bee = None

	def clear(self):
		self.ownerId = NEUTRAL_ID
		self.bee = None

	def getBeeBotId(self):
		return self.bee.botId if self.bee else NEUTRAL_ID
