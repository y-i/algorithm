from __future__ import annotations
from enum import IntEnum
from typing import Any, Callable, Type, TypeVar
from random import randint

T = TypeVar('T')
U = TypeVar('U')


class UnbalancedError(Exception):
    pass


class RotationError(BaseException):
    pass


class Color(IntEnum):
    BLACK = 0
    RED = 1


class Node:
    def __init__(self, key: Any, value: Any, parent: Type[Node], color: Color):
        self.key = key
        self.value = value
        self.color = color
        self.parent = parent
        if value is not None:
            self.left = Leaf(self)
            self.right = Leaf(self)
        else:
            self.left = None
            self.right = None

    def calc_black_nodes(self) -> int:
        left_num = self.left.calc_black_nodes()
        right_num = self.right.calc_black_nodes()
        if left_num != right_num:
            raise UnbalancedError()
        return left_num + (self.color == Color.BLACK)

    def print(self):
        def l_r(node):
            if node == node.parent.left:
                return 'left'
            elif node == node.parent.right:
                return 'right'
            else:
                return '???'

        self.left.print()
        print(self.key, self.color, l_r(self),
              self.parent.key if self.parent is not None else '-')
        self.right.print()

    def rotation_l(self):
        if self.parent.left == self:
            self.parent.left = self.right
        else:
            self.parent.right = self.right
        self.right.parent = self.parent
        self.parent = self.right
        right = self.right
        self.right = self.right.left
        self.right.parent = self
        right.left = self

    def rotation_r(self):
        if self.parent.left == self:
            self.parent.left = self.left
        else:
            self.parent.right = self.left
        self.left.parent = self.parent
        self.parent = self.left
        left = self.left
        self.left = self.left.right
        self.left.parent = self
        left.right = self


class RootNode(Node):
    def __init__(self):
        super().__init__(None, None, None, Color.BLACK)
        leaf = Leaf(self)
        self.left = leaf

    def calc_black_nodes(self) -> int:
        return self.left.calc_black_nodes()

    def print(self):
        self.left.print()

    def rotation_l(self):
        raise RotationError('Root node: Left rotation')

    def rotation_r(self):
        raise RotationError('Root node: Right rotation')


class Leaf(Node):
    def __init__(self, parent: Node):
        super().__init__(None, None, parent, Color.BLACK)

    def calc_black_nodes(self) -> int:
        return 1

    def print(self):
        pass

    def rotation_l(self):
        raise RotationError('Left rotation')

    def rotation_r(self):
        raise RotationError('Right rotation')


class RedBlackTree:
    def __init__(self, cmp: Callable[[T, T], int]):
        self.root = RootNode()
        self.cmp = cmp

    def isBalanced(self) -> bool:
        '''
        for debug
        '''
        try:
            self.root.calc_black_nodes()
        except UnbalancedError:
            return False
        return True

    def print(self):
        self.root.print()

    def balance(self, node: Node):
        changeNeeded = type(node.parent) is not RootNode and (
            node.color == node.parent.color)
        while changeNeeded:
            parent = node.parent
            grand_parent = parent.parent
            if type(grand_parent) is RootNode:
                return

            isDouble = False
            if parent == grand_parent.left:
                if node == parent.right:
                    parent.rotation_l()
                    isDouble = True
                grand_parent.rotation_r()
            else:
                if node == parent.left:
                    parent.rotation_r()
                    isDouble = True
                grand_parent.rotation_l()

            if type(node) is RootNode:
                return

            if not isDouble:
                node = node.parent
            node.color = Color.RED
            node.left.color = Color.BLACK
            node.right.color = Color.BLACK

            changeNeeded = type(
                node.parent) is not RootNode and node.color == node.parent.color

    def insert(self, key: T, value: U):
        # insert or update
        target_node = self.get_node(key)
        if type(target_node) is Node:
            target_node.value = value
            return
        node = Node(key, value, target_node.parent, Color.RED)

        if target_node == target_node.parent.left:
            target_node.parent.left = node
        else:
            target_node.parent.right = node

        # balance
        self.balance(node)

        self.root.left.color = Color.BLACK

    def get(self, key: T) -> U:
        node = self.get_node(key)
        return node.value

    def get_node(self, key: T) -> Node:
        node = self.root.left
        while type(node) is Node:
            cmp = self.cmp(node.key, key)
            if cmp == 0:
                return node
            elif cmp == 1:
                node = node.right
            elif cmp == -1:
                node = node.left

        return node

    def has(self, key: T) -> bool:
        return type(self.get_node(key)) is Node


if __name__ == "__main__":
    def cmp(l: int, r: int) -> int:
        if l < r:
            return 1
        elif l > r:
            return -1
        else:
            return 0
    tree = RedBlackTree(cmp)
    for i in range(100):
        k, v = randint(0, 100), randint(0, 1000)
        print(k, v)
        tree.insert(k, v)
        print(tree.isBalanced())

    print(tree.print())
    print(tree.isBalanced())
