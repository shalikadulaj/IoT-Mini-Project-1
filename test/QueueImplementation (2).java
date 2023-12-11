package oy.tol.tra;

/**
 * A simple implementation of the QueueInterface using an array.
 */
public class QueueImplementation<E> implements QueueInterface<E> {
    private static final int DEFAULT_CAPACITY = 10;

    private Object[] itemArray;
    private int front;
    private int rear;
    private int size;

    /**
     * Default constructor to initialize the queue with a default capacity.
     */
    public QueueImplementation() {
        this(DEFAULT_CAPACITY);
    }

    /**
     * Constructor to initialize the queue with a specified capacity.
     *
     * @param initialCapacity The initial capacity of the queue.
     */
    public QueueImplementation(int initialCapacity) {
        if (initialCapacity <= 0) {
            throw new IllegalArgumentException("Capacity must be greater than zero");
        }
        this.itemArray = new Object[initialCapacity];
        this.front = 0;
        this.rear = -1;
        this.size = 0;
    }

    @Override
    public int capacity() {
        return itemArray.length;
    }

    @Override
    public void enqueue(E element) throws QueueAllocationException, NullPointerException {
        if (element == null) {
            throw new NullPointerException("Element cannot be null");
        }

        if (size == itemArray.length) {
            // Perform reallocation if the queue is full
            reallocate();
        }

        rear = (rear + 1) % itemArray.length;
        itemArray[rear] = element;
        size++;
    }

    @Override
    public E dequeue() throws QueueIsEmptyException {
        if (isEmpty()) {
            throw new QueueIsEmptyException("queue is empty");
        }

        E element = (E) itemArray[front];
        front = (front + 1) % itemArray.length;
        size--;

        return element;
    }

    @Override
    public E element() throws QueueIsEmptyException {
        if (isEmpty()) {
            throw new QueueIsEmptyException("Queue is empty");
        }

        return (E) itemArray[front];
    }

    @Override
    public int size() {
        return size;
    }

    @Override
    public boolean isEmpty() {
        return size == 0;
    }

    @Override
    public void clear() {
        front = 0;
        rear = -1;
        size = 0;
    }
    
    @Override
    public String toString() {
        if (isEmpty()) {
            return "[]";
        }

        StringBuilder result = new StringBuilder("[");
        int i = front;
        do {
            result.append(itemArray[i]).append(", ");
            i = (i + 1) % itemArray.length;
        } while (i != (rear + 1) % itemArray.length);

        // Remove the trailing comma and space
        result.setLength(result.length() - 2);
        result.append("]");
        return result.toString();
    }



    // @Override
    // public String toString() {
    //     StringBuilder result = new StringBuilder("[");
    //     for (int i = front; i != rear; i = (i + 1) % itemArray.length) {
    //         result.append(itemArray[i]).append(", ");
    //     }
    //     if (!isEmpty()) {
    //         result.append(itemArray[rear]);
    //     }
    //     result.append("]");
    //     return result.toString();
    // }

    /**
     * Helper method to reallocate the array when it is full.
     */
    // private void reallocate() {
    //     int newCapacity = itemArray.length * 2;
    //     Object[] newArray = new Object[newCapacity];

    //     int index = 0;
    //     for (int i = front; i <= rear; i++) {
    //         newArray[index++] = itemArray[i % itemArray.length];
    //     }

    //     front = 0;
    //     rear = size - 1;
    //     itemArray = newArray;
    // }
    /**
 * Helper method to reallocate the array when it is full.
 */
    // private void reallocate() {
    //     int newCapacity = itemArray.length * 2;
    //     Object[] newArray = new Object[newCapacity];

    //     int index = 0;
    //     for (int i = front; i <= rear; i++) {
    //         newArray[index++] = itemArray[i % itemArray.length];
    //     }

    //     front = 0;
    //     rear = size - 1;
    //     itemArray = newArray;
    // }

    /**
 * Helper method to reallocate the array when it is full.
 */
    private void reallocate() {
        int newCapacity = itemArray.length * 2;
        Object[] newArray = new Object[newCapacity];

        int index = 0;
        int i = front;
        do {
            newArray[index++] = itemArray[i];
            i = (i + 1) % itemArray.length;
        } while (i != (rear + 1) % itemArray.length);

        front = 0;
        rear = size - 1;
        itemArray = newArray;
    }


}
