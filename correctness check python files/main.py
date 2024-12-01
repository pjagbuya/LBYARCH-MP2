import numpy as np

def read_data_from_file(filename, alt=False):
    with open(filename, 'r') as file:
        lines = file.readlines()
        data = []
        for line in lines:
            if alt:
                row = [int(num) for num in line.strip().split(", ")]
            else:
                row = [float(num) for num in line.strip().split()]
            data.append(row)
    return np.array(data)

def main():
    arr =read_data_from_file("input_gray.txt")
    new = (arr*255).round(0)
    print(new)
    given =read_data_from_file("output_test.txt", True)
    print(given)
    results = new ==given
    print(results)
    if(False in results):
        print("Results are False, wrong conversion")
    else:
        print("Results are True, right conversion")

    for i in range(0, 10):
        for j in range(0, 10):
            if given[i][j] != new[i][j]:
                print(f"Index row {i+1} at col {j+1}: {given[i][j]}!={new[i][j]}")

if __name__ == "__main__":
    main()