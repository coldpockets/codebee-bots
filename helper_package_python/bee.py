from helper_package_python.insect import Insect

class Bee(Insect):
	def __init__(self, id, botId, pos, face, pollen=0, count=1):
		super().__init__(id, botId, pos, face, pollen)
		self.count = count
