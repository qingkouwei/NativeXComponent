
type XComponentContextStatus = {
  hasDraw: boolean,
  hasChangeColor: boolean
};
export const add: (a: number, b: number) => number;
export const getStatus: () => XComponentContextStatus;
export const drawPattern: () => void;