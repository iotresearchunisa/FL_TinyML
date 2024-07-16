
# Federated learning for IoT devices: Enhancing TinyML with on-board training

The proliferation of the Internet of Things (IoT) with its innumerable applications, combined with the advent of machine learning (ML), has enabled the rapid growth of widespread and intelligent systems in a variety of fields. TinyML technology facilitates the implementation of locally operated machine learning (ML) algorithms on Internet of Things (IoT) devices. This paper presents a technique for training on-board ML algorithms on IoT devices, combining federated learning (FL) and transfer learning (TL). The experimental analysis compares the proposed approach to traditional FL solutions and combined techniques based on TensorFlow Lite, with a focus on classification and regression problems. The findings indicate that FLs with TLs achieve a higher level of accuracy (86.48%) than FLs without TLs in both classification and regression tasks (0.0201). We continue to analyze the time required for training and the energy consumed during inference on a variety of devices. Finally, we evaluated the impact of performance changes on unbalanced training data sets and demonstrated that, although they have a significant influence on accuracy, FL makes models more robust and can achieve accuracy comparable to that obtained for models trained on balanced data sets.
## Documentation

Raspberry ssh:
``` 
hostname: pi3
username: pi 
password: 123456789 
```

MQTT script on raspberry starts automatically and saves logs in:
```
/home/pi/log.txt
```
## Authors

- [@Massimo Ficco](https://docenti.unisa.it/058291/home)
- [@Antonio Guerriero](https://www.docenti.unina.it/#!/professor/414e544f4e494f47554552524945524f4752524e544e39324d3036483933314c/riferimenti)
- @Elio Milite
- [@Francesco Palmieri](https://docenti.unisa.it/026587/home)
- [@Roberto Pietrantuono](https://scholar.google.com/citations?hl=it&user=Htert2EAAAAJ)
- [@Stefano Russo](https://www.docenti.unina.it/#!/professor/53544546414e4f525553534f52535353464e36335032304638333959/riferimenti)
## Mention our work
```
@article{FICCO2024102189,
    title = {Federated learning for IoT devices: Enhancing TinyML with on-board training},
    journal = {Information Fusion},
    volume = {104},
    pages = {102189},
    year = {2024},
    issn = {1566-2535},
    doi = {https://doi.org/10.1016/j.inffus.2023.102189},
    url = {https://www.sciencedirect.com/science/article/pii/S1566253523005055},
    author = {M. Ficco and A. Guerriero and E. Milite and F. Palmieri and R. Pietrantuono and S. Russo},
    keywords = {Internet of things, Federated learning, Transfer learning, TinyML}
}


```
## Acknowledgements

The work by S. Russo was supported by the Sustainable Mobility National Research Center (MOST), Italy under Piano Nazionale di Ripresa e Resilienza (PNRR) – Missione 4 Componente 2, Investimento 1.4 – D.D. 1033 17/06/2022, CN00000023. The work by M. Ficco and F. Palmieri was partially supported by project SERICS (PE00000014) under the NRRP MUR program funded by the EU - NGEU. The work by M. Ficco is also part of the research activity realized within the project Federated Learning for Generative Emulation of Advanced Persistent Threats (FLEGREA), Bando PRIN 2022. All authors have read and agreed to the published version of the manuscript.