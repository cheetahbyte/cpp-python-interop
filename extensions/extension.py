import typing
from dataclasses import dataclass
import datetime

@dataclass
class ModuleData:
    took: int
    result: typing.Any


def fibonacci(n: int) -> int:
    if n == 0:
        return 0

    elif n == 1 or n == 2:
        return 1

    else:
        return fibonacci(n-1) + fibonacci(n-2)


def load(x: int) -> ModuleData:
    start = datetime.datetime.now()
    result = fibonacci(x)
    return ModuleData(took=(datetime.datetime.now() - start).microseconds, result=result)


if __name__ == "__main__":
    print(load(20))