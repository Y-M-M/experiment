import heapq
from collections import defaultdict

class HuffmanNode:
    def __init__(self, char, freq):
        self.char = char
        self.freq = freq
        self.left = None
        self.right = None

    def __lt__(self, other):
        return self.freq < other.freq

def build_huffman_tree(freq_table):
    # Create a priority queue (min-heap)
    heap = [HuffmanNode(char, freq) for char, freq in freq_table.items()]
    heapq.heapify(heap)

    # Combine nodes until we have one tree
    while len(heap) > 1:
        left = heapq.heappop(heap)
        right = heapq.heappop(heap)
        merged = HuffmanNode(None, left.freq + right.freq)
        merged.left = left
        merged.right = right
        heapq.heappush(heap, merged)

    return heap[0]  # The root of the Huffman tree

def generate_huffman_codes(root, prefix="", codebook={}):
    if root is not None:
        # Leaf node: store the code for the character
        if root.char is not None:
            codebook[root.char] = prefix
        generate_huffman_codes(root.left, prefix + "0", codebook)
        generate_huffman_codes(root.right, prefix + "1", codebook)
    return codebook

def calculate_frequency(file_path):
    freq_table = defaultdict(int)
    with open(file_path, 'r', encoding='utf-8') as f:
        text = f.read()
        for char in text:
            freq_table[char] += 1
    return freq_table

def calculate_compressed_length(huffman_codes, freq_table):
    total_bits = 0
    for char, code in huffman_codes.items():
        total_bits += len(code) * freq_table[char]
    return total_bits

def main(input_file):
    # Step 1: Calculate the frequency of characters
    freq_table = calculate_frequency(input_file)

    # Step 2: Calculate the total number of characters
    total_chars = sum(freq_table.values())
    print(f"Total number of characters in the input: {total_chars}")

    # Step 3: Print frequency of each character
    print("\nCharacter frequencies:")
    for char, count in freq_table.items():
        print(f"'{repr(char)}' : {count} times")

    # Step 4: Build the Huffman tree
    root = build_huffman_tree(freq_table)

    # Step 5: Generate Huffman codes
    huffman_codes = generate_huffman_codes(root)

    # Step 6: Calculate the compressed length
    compressed_length = calculate_compressed_length(huffman_codes, freq_table)

    # Step 7: Print the Huffman codes for each character
    print("\nCharacter | Frequency | Huffman Code")
    for char, code in huffman_codes.items():
        print(f"{repr(char)}        | {freq_table[char]}        | {code}")
    
    # Step 8: Print the total compressed length
    print("\nTotal compressed length (in bits):", compressed_length)

if __name__ == "__main__":
    # File path for the input file
    input_file = 'input.txt'
    main(input_file)
