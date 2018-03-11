from enum import IntEnum

NEUTRAL_ID = -1
BEE_POLLEN_AMOUNT = 5
HIVE_POLLEN_AMOUNT = 100
MAX_BEES = 100
MAX_POLLEN_FACTOR = 3

class Action(IntEnum):
    MOVE = 0
    SPAWN = 1
    CREATE_HIVE = 2
    MOVE_QUEEN = 3

class Face(IntEnum):
    UP = 0
    RIGHT = 1
    DOWN = 2
    LEFT = 3

class Flower(IntEnum):
    NONE = 0
    REGULAR = 1
    POTENT = 2

class Move(IntEnum):
    STAY = 0
    UP = 1
    RIGHT = 2
    DOWN = 3
    LEFT = 4
