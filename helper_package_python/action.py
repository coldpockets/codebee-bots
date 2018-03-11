from helper_package_python.position import Position
from helper_package_python.constants import Face

class Action:
	def __init__(self, type, pos=None, moveOrSpawnAmount=None, face=None):
		self.type = type
		self.pos = pos or Position()
		self.moveOrSpawnAmount = moveOrSpawnAmount or 0
		self.face = face or Face.UP
