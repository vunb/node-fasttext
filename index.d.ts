export declare class Classifier {
  constructor(modelFilename?: string);
  loadModel(modelFilename: string): Promise<any>;
  predict(sentence: string, k: number, callback?: DoneCallback): Promise<Array<{ label: string; value: number }>>;
  train(command: 'supervised' | 'skipgram' | 'cbow' | 'quantize', options: Options, callback?: DoneCallback): Promise<any>;
  quantize(options: Options, callback?: DoneCallback);
}

export declare class Query {
  constructor(modelFilename: string);
  nn(word: string, neighbors: number): Promise<Array<{ label: string; value: number }>>;
}

export interface Options {
  [key: string]: any;
  // The following arguments are mandatory
  input: string;              // training file path
  output: string;             // output file path

  // The following arguments are optional
  verbose: number;            // verbosity level [2]

  // The following arguments for the dictionary are optional
  minCount: number;           // minimal number of word occurrences [5]
  minCountLabel: number;      // minimal number of label occurrences [0]
  wordNgrams: number;         // max length of word ngram [1]
  bucket: number;             // number of buckets [2000000]
  minn: number;               // min length of char ngram [3]
  maxn: number;               // max length of char ngram [6]
  t: number;                  // sampling threshold [0.0001]
  label: string;              // labels prefix [__label__]

  // The following arguments for training are optional
  lr: number;                 // learning rate [0.05]
  lrUpdateRate: number;       // change the rate of updates for the learning rate [100]
  dim: number;                // size of word vectors [100]
  ws: number;                 // size of the context window [5]
  epoch: number;              // number of epochs [5]
  neg: number;                // number of negatives sampled [5]
  loss: 'softmax' | 'hs' | 'ls' | string; // loss function {ns, hs, softmax} [ns]
  thread: number;             // number of threads [12]
  pretrainedVectors: string;  // pretrained word vectors for supervised learning []
  saveOutput: boolean;        // whether output params should be saved [0]

  // The following arguments for quantization are optional
  cutoff: number;             // number of words and ngrams to retain [0]
  retrain: boolean;           // finetune embeddings if a cutoff is applied [0]
  qnorm: boolean;             // quantizing the norm separately [0]
  qout: boolean;              // quantizing the classifier [0]
  dsub: number;               // size of each sub-vector [2]
}

export interface DoneCallback {
  (error: any, result: any): void
}
