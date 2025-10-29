## Vector
```cpp
class Vector<T> {
	Object[] data = new Object[1];
	int size = 0;

	void push(T x) {
		if (size == data.length) {
			grow();
		}
		data[size++] = x;
	}

	T get(int i) {
		return (T) data[i];
	}

	void grow() {
		Object[] next = new Object[size * 2];
		for (int i = 0; i < size; i++) {
			next[i] = data[i];
		}
		data = next;
	}

}
```
