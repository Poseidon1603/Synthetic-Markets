### Mean Reversion

This readme pertains all the information relevant to the Mean Reversion Process attempted here

Just a disclaimer, the stuff written here is my understanding of this subject, revisions may be made later on, but I will leave all things untouched so anyone looking at this can also see how my understanding progressed.

We have setup an Ornstein-Uhlenbeck process, this process is a type of continuous-time stochastic process that models Mean-Reverting behaviour.
This means that unlike a Random-Walk or Brownian Motion which can drift indefinitely, the OU process reverts to a long term mean over time.

Mathematically, the OU process is a solution to a specific stochastic differential equation (SDE) that governs the mean-reversion behaviour. 

The SDE is given by:

    dX(t) = θ(μ - X(t))dt + σdW(t)

Where X(t) represents the stochastic process at time t, which really means its the value of X at time t. So at 3 seconds it could be 15.8.
μ is the long-term mean, the process should converge towards this mean
θ is the rate of mean reversion, which just means how strong is the force to move towards the mean.
σ is the volatility, but really is a magnitude of shocks, as will be explained later on 
dW(t) is a Weiner process or standard Brownian Motion

### The Intuition:

Every step, the next step is equal to some rate of mean reversion times the distance from the mean and then onto this we add a random value times some shock factor, the larger the mean reversion rate the more we move towards the mean. The larger the shock factor the more chance we have of suddenly jumping away from the mean, but we will eventually come back.

There should be some figures in which we diverge away from the mean permenantly, similarly there should be times where we converge on the mean and barely stray

How can we confirm that a stochastic process is a OU method?

The mean of all our price points should be approx our given mean, unless we have a drifting mean.


### Recap After More Research

The actual stochastic diffferential equation is a rule of the evolution of a variable over time and under randomness

Writing the SDE above you are saying that at each instance of t, the change in X is partly a deterministic pull towards μ. It's deterministic because there is no randomness involved with this part of the process, given the change in time, the mean, drift speed towards μ and the value of X at t, you can always know the value of this. The other part of this is a non-deterministic random noise, whose variance grows over time.

The solution to a stochastic differential equation is a random process, this random process is not a curve or a number or something you can point your finger on and say this will always be the solution, its a family of curves/data points, which havea a random aspect to them and by implication are not the same

Once you have a solution to the SDE, you can then take a sample from it, or a series of samples to produce a sample path, this sample path is one Monte-Carlo path of the solution.

Another important thing to go over what some of the properties of our Brownian Motion component are:
&emsp;    Zero Mean
&emsp;    Variance Proportional to time
&emsp;    Independant Increments
&emsp;    Nowhere differentiable

In descrete time:

    dW(t) ≈ sqrt(Δt)*Z where Z~N(0,1)



### Recap of the OU Process

The OU process is:

    dX(t) = θ(μ - X(t))dt + σ * dW(t)

where: 
&emsp;θ = Speed of mean reversion
&emsp;μ = the long term mean
&emsp;σ = The magnitude of shocks per time step

Properties of this include:
&emsp;    Stationary
&emsp;    Gaussian
&emsp;    Mean-reverting
&emsp;    Finite variance

To simulate a numerical approximation (discretized solution) we use the Euler-Maruyama method which looks like this:

    X(t + dt) = X(t) + θ(μ - X(t))dt + (σ + sqrt(dt) * Z(t)) where Z ~ N(0,1)



An important note here is that the Euler-Maruyama method is an approximation, for OU there is an exact discretization:

    X(t + dt) = μ +     (Χ(t) - μ)exp(-θdt) + (σ * Z * sqrt(1 - exp(-2θdt) / 2θ))

This removes time-step bias entirely.

There is actually a critical point here, which is that for Euler-Maruyama, it doesnt actually equal the OU process it basically is that as dt -> 0 EM -> OU

For an OU process, Euler-Maruyama systematically distorts:
&emsp;    - Mean Reversion speed:
&emsp;&emsp;       - Euler under reverts, the real effective speed is θ(effective) < θ
&emsp;&emsp;       - So the process looks stickier and reverts slower than specified
&emsp;    - Stationary Variance:
&emsp;&emsp;       - True OU variance is:

    Var(inf) = (σ^2 / 2θ)

&emsp;&emsp; - Euler produces:

    Var(inf) = σ^2 / 2θ - (θ^2)dt

&emsp;&emsp; As dt increases variance is inflated and can blow up if θdt > 2

&emsp; - Autocorrelation Structure

&emsp;&emsp; - True OU:

    Corr(X(t), X(t+dt)) = exp(-θκdt)

&emsp;&emsp; - Euler:

    (1 - θdt) ^ κ

Usings this, I'm going to rewrite the mean reversion process to use an exact discretization.