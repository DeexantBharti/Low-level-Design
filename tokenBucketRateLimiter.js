class tokenBucketLimiter{
  constructor(capacity, refillRate) {
    this.capacity = capacity;
    this.tokens = capacity;
    this.refillRate = refillRate;
    this.lastRefillTime = Date.now();
  }
  _refill() {
    const now = Date.now();
    const elapsedTime = now - this.lastRefillTime;
    const tokensToAdd = elapsedTime * this.refillRate;
    if (tokensToAdd > 0) {
      tokens = Math.min(this.capacity, tokensToAdd + tokens);
      this.lastRefillTime = Date.now();
    }
  }
  consume() {
    this._refill();
    if (this.tokens > 0) {
      this.tokens -= 1;
      return true;// request denied;
    }
    return false;// request denied 
  }
}

const limiter = new tokenBucketLimiter(10, 1 / 1000);
if (limiter.consume()) {
  console.log("request succedded");
}