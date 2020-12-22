class ClusterizationIndex
{
public:
	ClusterizationIndex();
	virtual ~ClusterizationIndex()=default;
	virtual double calculateIndex(Dataset dataset)const;
};