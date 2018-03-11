class Position:
	def __init__(self, x=0, y=0):
		self.x = x
		self.y = y

	def __eq__(self, other):
		return self.x == other.x and self.y == other.y

	def __ne__(self, other):
		return not self.__eq__(other)
