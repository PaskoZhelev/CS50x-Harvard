import nltk

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
        self.positives = set()
        self.negatives = set()
        with open(positives) as lines:
            for line in lines:
                if ((line == "") or (line.startswith(';'))):
                    continue
                else:
                    self.positives.add(line.strip())
                    
        with open(negatives) as lines:
            for line in lines:
                if ((line == "") or (line.startswith(';'))):
                    continue
                else:
                    self.negatives.add(line.strip())



    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""
        tokenizer = nltk.tokenize.TweetTokenizer()
        tokens = tokenizer.tokenize(text)
        sum = 0;
        for token in tokens:
          if token.lower() in self.positives:
              sum += 1
          elif token.lower() in self.negatives:
              sum -= 1
          
        return sum
