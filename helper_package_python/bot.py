from sys import stdin, stdout
import json

from helper_package_python.constants import ActionType, NEUTRAL_ID
from helper_package_python.map import Map
from helper_package_python.position import Position

class Bot:
	def __init__(self, name):
		self.name = name

	def run(self):
		self.getInit()

		self.sendInit(self.name)

		turn = 1
		while True:
			self.getTurn()

			actions = self.getActions()

			self.sendTurn(actions)

			turn += 1

	def getInit(self):
		self.id = int(self.getString())
		map = self.getString()

		pMap = json.loads(map)
		self.currMap = Map(map)

		self.flowerCells = []
		self.beeCells = []
		self.hiveCells = []

		for row in self.currMap.map:
			for item in row:
				if item.potency > 0:
					self.flowerCells.append(item)

	def sendInit(self, name):
		self.sendString(name)

	def getTurn(self):
		currState = self.getString()
		self.currMap.updateMap(currState)

		self.queenBee = next(queenBee for queenBee in self.currMap.queenBees if queenBee.botId == self.id)
		self.beeCells.clear()
		self.hiveCells.clear()
		self.totalBees = 0

		# Get all important cells
		for row in self.currMap.map:
			for item in row:
				bee = item.bee
				if bee and bee.botId == self.id:
					self.beeCells.append(item)
					self.totalBees += bee.count

				if item.ownerId == self.id:
					self.hiveCells.append(item)

	def sendTurn(self, actions):
		self.sendString(self.__serializeActions(actions))

	def getString(self):
		return stdin.readline()

	def sendString(self, output):
		stdout.write(str(output) + '\n')
		stdout.flush()

	def __serializeActions(self, actions):
		mappedActions = []
		for action in actions:
			mappedAction = {
				'type': action.type,
			}
			if action.type == ActionType.MOVE or action.type == ActionType.SPAWN:
				mappedAction['x'] = action.pos.x
				mappedAction['y'] = action.pos.y
				if action.type == ActionType.MOVE:
					mappedAction['move'] = action.moveOrSpawnAmount
				elif action.type == ActionType.SPAWN:
					mappedAction['amount'] = action.moveOrSpawnAmount
				mappedAction['face'] = action.face
			elif action.type == ActionType.MOVE_QUEEN:
				mappedAction['move'] = action.moveOrSpawnAmount
				mappedAction['face'] = action.face

			mappedActions.append(mappedAction)

		return json.dumps(mappedActions)

	def isBesideHiveOrQueen(self, pos):
		return self.isBeside(pos, self.queenBee.pos) or self.isBesideHive(pos)

	def isBesideHive(self, pos):
		for hiveCell in self.hiveCells:
			if self.isBeside(pos, hiveCell.pos):
				return True

		return False

	def isBeside(self, pos, target):
		return (pos == target) or \
			self.getBoundedPos(pos.x, pos.y + 1) == target or \
			self.getBoundedPos(pos.x, pos.y - 1) == target or \
			self.getBoundedPos(pos.x + 1, pos.y + 1) == target or \
			self.getBoundedPos(pos.x + 1, pos.y - 1) == target or \
			self.getBoundedPos(pos.x - 1, pos.y + 1) == target or \
			self.getBoundedPos(pos.x - 1, pos.y - 1) == target or \
			self.getBoundedPos(pos.x + 1, pos.y) == target or \
			self.getBoundedPos(pos.x - 1, pos.y) == target

	def isBesideEnemy(self, pos):
		return self.hasEnemy(pos) or \
			self.hasEnemy(self.getBoundedPos(pos.x + 1, pos.y)) or \
			self.hasEnemy(self.getBoundedPos(pos.x + 1, pos.y + 1)) or \
			self.hasEnemy(self.getBoundedPos(pos.x + 1, pos.y - 1)) or \
			self.hasEnemy(self.getBoundedPos(pos.x - 1, pos.y)) or \
			self.hasEnemy(self.getBoundedPos(pos.x - 1, pos.y + 1)) or \
			self.hasEnemy(self.getBoundedPos(pos.x - 1, pos.y - 1)) or \
			self.hasEnemy(self.getBoundedPos(pos.x, pos.y + 1)) or \
			self.hasEnemy(self.getBoundedPos(pos.x, pos.y - 1))

	def hasEnemy(self, pos):
		cellBotId = self.currMap.map[pos.y][pos.x].getBeeBotId()
		return cellBotId != NEUTRAL_ID and cellBotId != self.id

	def getBoundedPos(self, x, y):
		return Position((self.currMap.width + x) % self.currMap.width, (self.currMap.height + y) % self.currMap.height)

	def getMinPath(self, start, end):
		return self.currMap.getPath(start, end)
