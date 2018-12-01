'''training_data = [
    ['Sol','Caliente','Alta','Debil','No'],
    ['Sol','Caliente','Alta','Fuerte','No'],
    ['Cubierto','Caliente','Alta','Debil','Si'],
    ['Lluvia','Normal','Alta','Debil','Si'],
    ['Lluvia','Frio','Normal','Debil','No'],
    ['Lluvia','Frio','Normal','Fuerte','No'],
    ['Cubierto','Frio','Normal','Fuerte','Si'],
    ['Sol','Normal','Alta','Debil','No'],
    ['Sol','Frio','Normal','Debil','Si'],
    ['Lluvia','Normal','Normal','Debil','Si'],
]'''
import csv

def fixText(text):
    row = []
    z = text.find(',')
    if z == 0:  row.append('')
    else:   row.append(text[:z])
    for x in range(len(text)):
        if text[x] != ',':  pass
        else:
            if x == (len(text)-1):  row.append('')
            else:
                if ',' in text[(x+1):]:
                    y = text.find(',', (x+1))
                    c = text[(x+1):y]
                else:   c = text[(x+1):]
                row.append(c)
    return row

def getHeader(file):
    f1=open(file,"r")
    text=f1.readline()
    row=fixText(text)
    return row

def createTuple(oldFile,skip):
    ## oldFile is filename (e.g. 'sheet.csv')
    f1 = open(oldFile, "r")
    tup = []
    for x in range (1,skip):
        text=f1.readline();
    while 1:
        text = f1.readline()
        if text == "":  break
        else:   pass
        if text[-1] == '\n':
            text = text[:-1]
        else:   pass
        row = fixText(text)
        tup.append(row)
    return tup

training_data=createTuple("training.csv",1)
testing_data=createTuple("testing.csv",0)

#Atributos para imprimir (el ultimo sera el que se prediga)
header = getHeader("training.csv")

#Input de atributos
'''
print (header[0])
tiempo=input("  ->")
print (header[1])
temperatura=input(" ->")
print (header[2])
humedad=input(" ->")
print (header[3])
viento=input(" ->")
'''

#devuelve los posibles valores para cada atributo
def unique_vals(rows, col):
    return set([row[col] for row in rows])

#Devuelve la cuenta de las ocurrencias de cada posible valor en el label
def class_counts(rows):
    counts = {}
    for row in rows:
        label = row[-1]
        if label not in counts:
            counts[label] = 0
        counts[label] += 1
    return counts


def is_numeric(value):
    return isinstance(value, int) or isinstance(value, float)

class Question:

    def __init__(self, column, value):
        self.column = column
        self.value = value

    def match(self, example):
        val = example[self.column]
        if is_numeric(val):
            return val >= self.value
        else:
            return val == self.value

    def __repr__(self):
        condition = "=="
        if is_numeric(self.value):
            condition = ">="
        return "Es %s %s %s?" % (
            header[self.column], condition, str(self.value))

def partition(rows, question):
    true_rows, false_rows = [], []
    for row in rows:
        if question.match(row):
            true_rows.append(row)
        else:
            false_rows.append(row)
    return true_rows, false_rows

def gini(rows):
    counts = class_counts(rows)
    impurity = 1
    for lbl in counts:
        prob_of_lbl = counts[lbl] / float(len(rows))
        impurity -= prob_of_lbl**2
    return impurity



def info_gain(left, right, current_uncertainty):
    p = float(len(left)) / (len(left) + len(right))
    return current_uncertainty - p * gini(left) - (1 - p) * gini(right)


def find_best_split(rows):
    best_gain = 0
    best_question = None
    current_uncertainty = gini(rows)
    n_features = len(rows[0]) - 1
    for col in range(n_features):
        values = set([row[col] for row in rows])
        for val in values:
            question = Question(col, val)
            true_rows, false_rows = partition(rows, question)
            if len(true_rows) == 0 or len(false_rows) == 0:
                continue
            gain = info_gain(true_rows, false_rows, current_uncertainty)
            if gain >= best_gain:
                best_gain, best_question = gain, question
    return best_gain, best_question

class Leaf:
    def __init__(self, rows):
        self.predictions = class_counts(rows)


class Decision_Node:
    def __init__(self,
                 question,
                 true_branch,
                 false_branch):
        self.question = question
        self.true_branch = true_branch
        self.false_branch = false_branch


def build_tree(rows):
    gain, question = find_best_split(rows)
    if gain == 0:
        return Leaf(rows)
    true_rows, false_rows = partition(rows, question)
    true_branch = build_tree(true_rows)
    false_branch = build_tree(false_rows)
    return Decision_Node(question, true_branch, false_branch)

def print_tree(node, spacing="      "):
    if isinstance(node, Leaf):
        print (spacing + "Prediccion", node.predictions)
        return
    print (spacing + str(node.question))
    print (spacing + '--> Si:')
    print_tree(node.true_branch, spacing + "  ")
    print (spacing + '--> No:')
    print_tree(node.false_branch, spacing + "  ")


def classify(row, node):
    if isinstance(node, Leaf):
        return node.predictions
    if node.question.match(row):
        return classify(row, node.true_branch)
    else:
        return classify(row, node.false_branch)

def print_leaf(counts):
    total = sum(counts.values()) * 1.0
    probs = {}
    for lbl in counts.keys():
        probs[lbl] = str(int(counts[lbl] / total * 100)) + "%"
    return probs

if __name__ == '__main__':

    decision_tree = build_tree(training_data)
    print_tree(decision_tree)

    for row in range (0,len(testing_data)):
        for col in range (0,len(testing_data[row])):
            print (header[col],": ",testing_data[row][col],end=", ")
        print ("Prediccion: %s" %
               (print_leaf(classify(testing_data[row], decision_tree))))
