from nltk.corpus import brown

def generate_noun_list():
    nouns = set()
    min_count_per_char = 15
    target_count = 2000

    for word in brown.words(categories='news'):
        if word.isalpha() and word.islower() and len(word) >= 3:
            nouns.add(word)

        if len(nouns) >= target_count and all(len([n for n in nouns if len(n) >= char]) >= min_count_per_char for char in range(ord('a'), ord('z') + 1)):
            break

    return sorted(list(nouns))

if __name__ == "__main__":
    nouns = generate_noun_list()

    with open("common_nouns.txt", "w") as file:
        file.write("\n".join(nouns))
