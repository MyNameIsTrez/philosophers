from enum import Enum, auto


class Fork(Enum):
	LEFT = auto()
	RIGHT = auto()


class Actions(Enum):
	NONE = auto()

	GRAB_LEFT_FORK = auto()
	GRAB_RIGHT_FORK = auto()

	DROP_LEFT_FORK = auto()
	DROP_RIGHT_FORK = auto()


class Philosopher:
	def __init__(self, index, philosopher_count):
		self.index = index

		self.left_fork_index = index
		self.right_fork_index = (index + 1) % philosopher_count

		self.holding_left_fork = False
		self.holding_right_fork = False


	def act(self, action, forks):
		if action == Actions.NONE:
			return True
		elif action == Actions.GRAB_LEFT_FORK:
			if self.grab_left_fork(forks):
				return True
		elif action == Actions.GRAB_RIGHT_FORK:
			if self.grab_right_fork(forks):
				return True
		elif action == Actions.DROP_LEFT_FORK:
			if self.drop_left_fork(forks):
				return True
		elif action == Actions.DROP_RIGHT_FORK:
			if self.drop_right_fork(forks):
				return True


	def undo_act(self, action, forks):
		if action == Actions.NONE:
			return
		elif action == Actions.GRAB_LEFT_FORK:
			self.drop_left_fork(forks)
		elif action == Actions.GRAB_RIGHT_FORK:
			self.drop_right_fork(forks)
		elif action == Actions.DROP_LEFT_FORK:
			self.grab_left_fork(forks)
		elif action == Actions.DROP_RIGHT_FORK:
			self.grab_right_fork(forks)


	def grab_left_fork(self, forks):
		if forks[self.left_fork_index] == False and self.wants_to_grab(Fork.LEFT):
			self.holding_left_fork = True
			forks[self.left_fork_index] = True
			return True


	def grab_right_fork(self, forks):
		if forks[self.right_fork_index] == False and self.wants_to_grab(Fork.RIGHT):
			self.holding_right_fork = True
			forks[self.right_fork_index] = True
			return True


	def drop_left_fork(self, forks):
		if forks[self.left_fork_index] == True and self.holding_left_fork:
			forks[self.left_fork_index] = False
			self.holding_left_fork = False
			return True


	def drop_right_fork(self, forks):
		if forks[self.right_fork_index] == True and self.holding_right_fork:
			forks[self.right_fork_index] = False
			self.holding_right_fork = False
			return True


	def wants_to_grab(self, grabbing_fork):
		self.grabbing_strategy_always_left(grabbing_fork)
		# self.grabbing_strategy_odd_even(grabbing_fork)


	def grabbing_strategy_always_left(self, grabbing_fork):
		if self.holding_left_fork:
			if self.holding_right_fork:
				return False
			else:
				if grabbing_fork == Fork.RIGHT:
					return True
				else:
					return False
		else:
			if grabbing_fork == Fork.LEFT:
				return True
			else:
				return False


	def grabbing_strategy_odd_even(self, grabbing_fork):
		"""
		Even philosophers should pick up the left fork first
		Odd philosophers should pick up the right fork first
		"""
		if self.index % 2 == 0:
			if self.holding_left_fork:
				if self.holding_right_fork:
					return False
				else:
					if grabbing_fork == Fork.RIGHT:
						return True
					else:
						return False
			else:
				if grabbing_fork == Fork.LEFT:
					return True
				else:
					return False
		else:
			if self.holding_right_fork:
				if self.holding_left_fork:
					return False
				else:
					if grabbing_fork == Fork.LEFT:
						return True
					else:
						return False
			else:
				if grabbing_fork == Fork.RIGHT:
					return True
				else:
					return False


def depth_first_search(forks, philosophers, depth, max_depth):
	for philosopher in philosophers:
		for action in Actions:
			acted = philosopher.act(action, forks)

			if depth + 1 < max_depth:
				depth_first_search(forks, philosophers, depth + 1, max_depth)

			if acted:
				philosopher.undo_act(action, forks)


def iterative_deepening_depth_first_search(forks, philosophers):
	max_depth = 1

	while True:
		print(max_depth)
		depth_first_search(forks, philosophers, 0, max_depth)
		max_depth += 1


def main():
	time_to_eat = 2
	time_to_sleep = 1

	philosopher_count = 2

	forks = [ False ] * philosopher_count

	philosophers = []
	for index in range(philosopher_count):
		philosopher = Philosopher(index, philosopher_count)
		philosophers.append(philosopher)

	iterative_deepening_depth_first_search(forks, philosophers)


if __name__ == "__main__":
	main()
