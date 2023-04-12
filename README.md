# Multi-Source Prim-Dijkstra  

<span style="color:red">***UPDATE***</span>
Our MSPD Machine Learning [Contest](https://github.com/TILOS-AI-Institute/Multi-Source-Prim-Dijkstra/tree/main/contest) is now open for registration !!!   
Thanks to [The OpenROAD Project](https://theopenroadproject.org), the contest has over $7000 in available prizes!      

   



- [src/](src/) contains the implementations of MSS and MSPD for underlying PDRev (PD+HVW+DAS) and STT (stt+DAS) Steiner constructions.  

- [contest/](contest/) contains the contents and descriptions for our **machine learning contest**, which launches on April 5, 2023 and aims to inspire an ML-based solution for predicting good combinations of sources to use in MSPD with the STT construction. More than $7500 in prizes are available!

- [LUTs/](LUTs) contains a python script (LUT.py) that gives experimentally-determined normalized cost, normalized skew, variance in normalized cost, and variance in normalized skew, for given  $N$, $AR$, $p$, and $\alpha$ parameters. This corresponds to Section 3 in our ISQED-2023 [paper](https://vlsicad.ucsd.edu/Publications/Conferences/397/c397.pdf).  


When referring to either our paper or contest, please cite as shown with the following bibtex:  
```txt
@inproceedings{ISQED2023,
  author    = {Kahng, Andrew B. and Thumathy, Shreyas and Woo, Mingyu},
  title     = {An Effective Cost-Skew Tradeoff Heuristic for VLSI Global Routing},
  booktitle = {2023 International Symposium on Quality Electronic Design (ISQED)},
  year      = {2023},
  pages     = {1-8}
}
```
