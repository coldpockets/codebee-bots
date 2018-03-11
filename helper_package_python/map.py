from helper_package_python.constants import Move

from helper_package_python.bee import Bee
from helper_package_python.cell import Cell
from helper_package_python.position import Position
from helper_package_python.queen_bee import QueenBee

import json
from random import randint

JSON_INDICES = {
	'HIVE': {
		'X': 0,
		'Y': 1,
		'OWNER_ID': 2,
	},
	'INSECT': {
		'X': 0,
		'Y': 1,
		'ID': 2,
		'BOT_ID': 3,
		'FACE': 4,
		'POLLEN': 5,
		'COUNT': 6,
	},
	'STATE': {
		'BEES': 0,
		'QUEENS': 1,
		'HIVES': 2,
	},
}

class Path:
	def __init__(self, d=None, m=None):
		self.distance = d or 0
		self.move = m or Move.UP

class Map:
	def __init__(self, pollenMap):
		pMap = json.loads(pollenMap)
		
		self.height = len(pMap)
		self.width = len(pMap[0])

		# Initialize map using pollenMap
		self.map = []
		for y in range(0, self.height):
			self.map.append([])
			for x in range(0, self.width):
				self.map[y].append(Cell(Position(x, y), pMap[y][x]))

		self.queenBees = []

	def updateMap(self, newState):
		pState = json.loads(newState)
		pBees = pState[JSON_INDICES['STATE']['BEES']]
		pQueens = pState[JSON_INDICES['STATE']['QUEENS']]
		pHives = pState[JSON_INDICES['STATE']['HIVES']]

		for row in self.map:
			for item in row:
				item.clear()

		# clear queenBees list
		self.queenBees.clear()

		# set bees
		for pBee in pBees:
			bee = Bee(
				pBee[JSON_INDICES['INSECT']['ID']],
				pBee[JSON_INDICES['INSECT']['BOT_ID']],
				Position(pBee[JSON_INDICES['INSECT']['X']], pBee[JSON_INDICES['INSECT']['Y']]),
				pBee[JSON_INDICES['INSECT']['FACE']],
				pBee[JSON_INDICES['INSECT']['POLLEN']],
				pBee[JSON_INDICES['INSECT']['COUNT']],
			)
			self.map[bee.pos.y][bee.pos.x].bee = bee

		# set queen bees
		for pQueen in pQueens:
			self.queenBees.append(QueenBee(
					pQueen[JSON_INDICES['INSECT']['ID']],
					pQueen[JSON_INDICES['INSECT']['BOT_ID']],
					Position(pQueen[JSON_INDICES['INSECT']['X']], pQueen[JSON_INDICES['INSECT']['Y']]),
					pQueen[JSON_INDICES['INSECT']['FACE']],
					pQueen[JSON_INDICES['INSECT']['POLLEN']],
			))

		# set hives
		for pHive in pHives:
			self.map[pHive[JSON_INDICES['HIVE']['Y']]][pHive[JSON_INDICES['HIVE']['X']]].ownerId = pHive[JSON_INDICES['HIVE']['OWNER_ID']]

	def getPath(self, start, end):
		path = Path()
		downDist = (self.height + end.y - start.y) % self.height
		upDist = (self.height + start.y - end.y) % self.height
		rightDist = (self.width + end.x - start.x) % self.width
		leftDist = (self.width + start.x - end.x) % self.width

		move1 = Move.STAY
		move2 = Move.STAY
		if end.y != start.y:
			if downDist > upDist:
				move1 = Move.UP
			else:
				move1 = Move.DOWN
			path.move = move1

		if end.x != start.x:
			if rightDist > leftDist:
				move2 = Move.LEFT
			else:
				move2 = Move.RIGHT
			path.move = move2

		if move1 != Move.STAY and move2 != Move.STAY:
			path.move = move1 if randint(0, 1) == 0 else move2

		path.distance = min(rightDist, leftDist) + min(downDist, upDist)

		return path
