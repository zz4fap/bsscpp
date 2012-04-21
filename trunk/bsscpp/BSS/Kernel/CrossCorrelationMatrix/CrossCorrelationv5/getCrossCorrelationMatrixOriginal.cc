//INPUT: y1[] and y2[] vectors with N+D-1 samples.
template <class T>
void CrossCorrelationMatrix<T>::getCrossCorrelationMatrix(T y1[], T y2[], Matrix<T>& CMM)
{
	unsigned int tempN = 2*N;

	T ans[4*tempN];

	T* data1 = new T[tempN];
	T* data2 = new T[tempN];

	memset(data1,0,sizeof(T)*tempN);
	memset(data2,0,sizeof(T)*tempN);

	memcpy((data1+(N-(D-1))),y1,(sizeof(T)*(N+D-1)));

	for(int i = 0; i < this->D; i++) // Iterate over all rows: 0 up to (D-1)
	{
		memcpy((data2+N),(y2+i),(sizeof(T)*N));

		this->corr->doCorrelation(data1, data2, tempN, 2*tempN, ans);

		for(int j = 0; j < this->D; j++)
		{
			if(j == 0)
				CMM(j,(D-1)-i) = tempN*ans[0];
			else
				CMM(j,(D-1)-i) = tempN*ans[4*tempN-2*j];
		}
	}

	delete [] data1;
	delete [] data2;
}
